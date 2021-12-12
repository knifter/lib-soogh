#include "treemenu.h"

#include <soogh.h>

#include <tools-log.h>

/*
Class hierarchy:
	MenuItem 			(drawable: draw_item, draw_open)
		SubMenu 		(actual menu)
			TreeMenu	(Top most Menu)
		FloatField 		(float editor)
		SwitchField 	(on/off boolean)
		ListField 		(picklist)
*/

/*** MenuItem ***************************************************************************************/
MenuItem::MenuItem(MenuItem *parent, const char *text) : _parent(parent), _text(text)
{
	// DBG("CONSTRUCT(%s) = %p", _text, this);
	if(_parent)
		_parent->appendChild(this);
};

MenuItem::~MenuItem()
{
	// This will recursively(!) delete all children, bottom up.
	for(auto child: _children)
		delete child;

	if(_open)
	{
		WARNING("Destroying open object.");
	};
	// DBG("DESTROY(%s) = %p", _text, this);
};

MenuItem* MenuItem::parent()
{
	return _parent;
};

MenuItem* MenuItem::root()
{
	MenuItem* root = this;
	while(root->_parent != nullptr)
		root = root->_parent;
	return root;
};

void MenuItem::appendChild(MenuItem* child)
{
	if(!child)
	{
		ERROR("child == null");
		return;
	};
	_children.push_back(child);
};

void MenuItem::open()
{
	if(_open)
		return;
	// DBG("open(%s)", _text);
	// close other opened siblings
	if(_parent)
		_parent->close_children();

	draw_open();
	_open = true;
};

void MenuItem::close_children()
{
	// DBG("close_children(of %s)", _text);
	// propagate close through all children as well
	for(auto child: _children)
	{
		if(child->isOpen())
		{
			// DBG("%s: close child: %s", this->_text, child->_text);
			child->close();
		};
	};

};

void MenuItem::close()
{
	if(!_open)
		return;

	// DBG("close(%s)", _text);

	// make sure children are closed
	close_children();

	draw_close();
	
	// Call on-close event callback
	if(_close_cb)
		_close_cb(this, _close_data);

	// and then me
	_open = false;
};

bool MenuItem::isOpen() 
{ 
	return _open; 
};

void MenuItem::onClose(treemenu_cb_t *func, void* user_data) 
{ 
	_close_cb = func;
	_close_data = user_data; 
};

void MenuItem::set_group(lv_group_t *group)
{
	TreeMenu* menu = static_cast<TreeMenu*>(this->root());
	menu->group = group;
};


/*** Separator ***************************************************************************************/
void MenuSeparator::draw_btn(lv_obj_t *lv_list)
{
	lv_list_add_text(lv_list, _text);
};

/*** BooleanField ***************************************************************************************/
void BooleanField::draw_btn(lv_obj_t *lv_list)
{
	lv_obj_t *btn = lv_list_add_btn(lv_list, nullptr, _text);
	lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW_WRAP);
	lv_obj_set_style_pad_row(btn, 3, 0);

	switch(_type)
	{
		case BOOLTYPE_CHECKBOX:  
			_sw = lv_checkbox_create(btn); 
			lv_checkbox_set_text_static(_sw, ""); 
			break;
		case BOOLTYPE_SWITCH:	 _sw = lv_switch_create(btn); break;
		default: return;
	};
	
	// lv_obj_set_flex_grow(_sw, 1);
	// lv_obj_set_style_min_height(_sw, 0, 0);
	lv_obj_set_size(_sw, 40, 20);

	if(*value == true)
    	lv_obj_add_state(_sw, LV_STATE_CHECKED);

	lv_obj_add_event_cb(_sw, click_cb, LV_EVENT_CLICKED, this);
	lv_obj_add_event_cb(btn, click_cb, LV_EVENT_CLICKED, this);
};
/* static */ void BooleanField::click_cb(lv_event_t *e)
{
	BooleanField* me = static_cast<BooleanField*>(e->user_data);

	(*(me->value)) = !(*(me->value));
	if(*(me->value))
    	lv_obj_add_state(me->_sw, LV_STATE_CHECKED);
	else
		lv_obj_clear_state(me->_sw, LV_STATE_CHECKED);
};

/*** ActionItem ***************************************************************************************/
ActionField::ActionField(MenuItem *parent, const char *text, treemenu_cb_t *func, void* data) : MenuItem(parent, text)
{
	_change_cb = func; 
	_change_data = data;
};

void ActionField::draw_btn(lv_obj_t *lv_list)
{
	lv_obj_t *btn = lv_list_add_btn(lv_list, nullptr, _text);
	lv_obj_add_event_cb(btn, click_cb, LV_EVENT_CLICKED, this);
};
/* static */ void ActionField::click_cb(lv_event_t *e)
{
	ActionField* me = static_cast<ActionField*>(e->user_data);

	if(me->_change_cb)
		me->_change_cb(me, me->_change_data);
};

/*** FloatField ***************************************************************************************/
void FloatField::draw_btn(lv_obj_t *lv_list)
{
	// TODO: can this be moved to MenuItem?
	lv_obj_t *btn = lv_list_add_btn(lv_list, nullptr, _text);
	lv_obj_add_event_cb(btn, click_cb, LV_EVENT_CLICKED, this);
	lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW_WRAP);
	lv_obj_set_style_pad_row(btn, 3, 0);

	_btn_lbl = lv_label_create(btn);
	lv_label_set_text_fmt(_btn_lbl, "%.02f", *value);
	// lv_obj_set_flex_grow(_btn_lbl, 1);
	lv_obj_set_style_text_color(_btn_lbl, COLOR_GREY, 0);
};

/* static */ void FloatField::click_cb(lv_event_t *e) // static
{
	FloatField* me = static_cast<FloatField*>(e->user_data);
	me->open();
};

int FloatField::digits()
{
	int min_digits = ceil(log10(min_value));
	int max_digits = ceil(log10(max_value));
	DBG("mindig = %d, maxdig = %d", min_digits, max_digits);
	return max(min_digits, max_digits) + decimals;
};

void FloatField::draw_open()
{
	// get coords of label
	lv_area_t coor;
	lv_obj_get_coords(_btn_lbl, &coor);

	// draw (floating) spinbox right over label
	_spinbox = lv_spinbox_create(lv_layer_top());
	lv_obj_set_pos(_spinbox, coor.x1-5, coor.y1-5);
	// lv_obj_set_style_pad_all(_spinbox, 3, 0);

	lv_spinbox_set_range(_spinbox, min_value * decimals, max_value*decimals);
	int digits = this->digits();
	lv_spinbox_set_digit_format(_spinbox, digits, digits - decimals);
	lv_spinbox_set_value(_spinbox, *value * 100);

	// And floating buttons just below the spinbox
	_btns = lv_btnmatrix_create(lv_layer_top());
	lv_coord_t x, y, w, h;
	x = coor.x1;
	h = 50;
	w = DISPLAY_WIDTH - coor.x1 - 10;

	if(coor.y1 < DISPLAY_HEIGHT /2)
	{
		// place below spinbox
		y = coor.y2;
	}else{
		// place above spinbox
		y = coor.y1 - h;
	};

	// DBG("btns xywh = %d %d %d %d", x, y, w, h);
	lv_obj_set_size(_btns, w, h);
	lv_obj_set_pos(_btns, x, y);
	lv_obj_set_style_pad_all(_btns, 3, 0);

	static const char * map[] = {
		LV_SYMBOL_LEFT, LV_SYMBOL_MINUS, LV_SYMBOL_OK, LV_SYMBOL_PLUS, LV_SYMBOL_RIGHT, "" };
	lv_btnmatrix_set_map(_btns, map);
	lv_btnmatrix_set_btn_width(_btns, 2, 2);
	lv_obj_add_event_cb(_btns, btns_cb, LV_EVENT_VALUE_CHANGED, this);
};

void FloatField::draw_close()
{
	lv_label_set_text_fmt(_btn_lbl, "%.02f", *value);

	lv_obj_del(_spinbox);
	lv_obj_del(_btns);
};

/* static */ void FloatField::btns_cb(lv_event_t * e)
{
	// lv_event_code_t code = lv_event_get_code(e);
	lv_obj_t * obj = lv_event_get_target(e);
	uint32_t id = lv_btnmatrix_get_selected_btn(obj);
	FloatField* me = static_cast<FloatField*>(e->user_data);
	switch(id)
	{
		case 0: lv_spinbox_step_prev(me->_spinbox); break;
		case 1: lv_spinbox_decrement(me->_spinbox); break;
		case 2: DBG("Close"); me->close(); break;
		case 3: lv_spinbox_increment(me->_spinbox); break;
		case 4: lv_spinbox_step_next(me->_spinbox); break;
		default: DBG("ID = %d", id); break;
	};
	*(me->value) = static_cast<float>(lv_spinbox_get_value(me->_spinbox)) / pow(10, (me->decimals));
};

/*** SubMenu ***************************************************************************************/
void SubMenu::draw_open()
{

	lv_obj_t *lv_parent = lv_layer_top();

	// FIXME leaks:
	lv_group_t* grp = lv_group_create();
	lv_group_set_editing(grp, false);
	// lv_group_set_focus_cb(grp, group_focus_cb);
    lv_group_set_default(grp);
	// _gui.pushGroup(grp);
	set_group(grp);

	_list = lv_list_create(lv_parent);
	lv_obj_align(_list, LV_ALIGN_LEFT_MID, 0, 0);
	lv_obj_set_size(_list, LV_PCT(80), LV_PCT(100));

	if(!_parent)
	{
		// Exit button
		lv_obj_t *btn = lv_list_add_btn(_list, LV_SYMBOL_CLOSE, "Close");
		lv_obj_add_event_cb(btn, SubMenu::close_cb, LV_EVENT_CLICKED, this);
	};

	for(auto child: _children)
		child->draw_btn(_list);
};

void SubMenu::draw_close()
{
	lv_obj_del(_list);	
};

MenuSeparator* SubMenu::addSeparator(const char* text)
{
	return new MenuSeparator(this, text);
};

SubMenu* SubMenu::addSubMenu(const char* text)
{
	return new SubMenu(this, text);
};

FloatField* SubMenu::addFloat(const char* name, float* f)
{
	return new FloatField(this, name, f);
};

ActionField* SubMenu::addAction(const char* name, treemenu_cb_t *func, void *data)
{
	return new ActionField(this, name, func, data);
};

BooleanField* SubMenu::addSwitch(const char* name, bool *b)
{
	return new BooleanField(this, name, b, BooleanField::BOOLTYPE_SWITCH);
};

BooleanField* SubMenu::addCheckbox(const char* name, bool *b)
{
	return new BooleanField(this, name, b, BooleanField::BOOLTYPE_CHECKBOX);
};

void SubMenu::draw_btn(lv_obj_t *lv_list)
{
	lv_obj_t *btn = lv_list_add_btn(lv_list, LV_SYMBOL_RIGHT, _text);
	btn = btn;
};

/*static*/ void SubMenu::close_cb(lv_event_t *e)
{
	SubMenu* me = static_cast<SubMenu*>(e->user_data);
	me->close();
};

/*** Root ***************************************************************************************/
TreeMenu::~TreeMenu()
{
	// We need to close (remove widgets) the menu before free-ing it
	// But needs to be done here on the root menu and derived class: the vtable is gone in ~MenuItem
	close();
};
