#ifndef __TREEMENU_H
#define __TREEMENU_H

#include <soogh.h>

// #include <memory>
#include <vector>

#include <tools-log.h>
#include <tools-nocopy.h>


/*
	MenuItem (drawable: draw_item, draw_open)
		SubMenu
			TreeMenu(Menu)
		FloatItem
		SwitchItem
		ListItem
*/

class MenuItem;

typedef void treemenu_cb_t(MenuItem* item, void* user_data);

typedef std::vector<MenuItem*> MenuItemArray;
class MenuItem //: public NonCopyable
{
	public:
		MenuItem(MenuItem *parent, const char *text = "<none>");
		virtual ~MenuItem();

		virtual void close();
		virtual void open();
		virtual bool isOpen();
		virtual void onClose(treemenu_cb_t func, void* user_data = nullptr) { _close_cb = func, _close_data = user_data; };
		// virtual void onChange(treemenu_cb_t func, void* user_data = nullptr);

	// Should be protected
		virtual void close_children();

		virtual void draw_btn(lv_obj_t *lv_list) = 0;
	protected:
		virtual void draw_open() = 0;
		virtual void draw_close() = 0;

		MenuItem* parent();
		MenuItem* root();
		void appendChild(MenuItem* child);

	protected:
		
		MenuItem *_parent = nullptr;
		MenuItemArray _children;

		const char* _text;
		bool _open = false;
		// lv_obj_t *_obj = nullptr;

		// Callbacks
		treemenu_cb_t 	*_close_cb = nullptr;
		void 			*_close_data = nullptr;
		// treemenu_cb_t 	*_change_cb = nullptr;
		// void 			*_change_data = nullptr;
};

class MenuSeparator : public MenuItem
{
	public:
		MenuSeparator(MenuItem *parent, const char *text) : MenuItem(parent, text) {};

	protected:
		void draw_btn(lv_obj_t *lv_list);
		void draw_open() {};
		void draw_close() {};
};

class FloatField : public MenuItem
{
 	public:
 		FloatField(MenuItem *parent, const char *text, float *f) : MenuItem(parent, text), value(f) { };

	public: // member vars
		float *value;
		float min_value = 0;
		float max_value = 10;
		int decimals = 2;

	protected:
		void draw_btn(lv_obj_t *lv_list);
		void draw_open();
		void draw_close();

	private: // Callbacks
		static void click_cb(lv_event_t *e);
		static void btns_cb(lv_event_t * e);
		int digits();
	private:
		//draw_btn
		lv_obj_t *_btn = nullptr;

		//draw_open/close
		lv_obj_t *_btns = nullptr;
		lv_obj_t *_spinbox = nullptr;
		lv_obj_t *_btn_lbl = nullptr;
};

class SubMenu : public MenuItem
{
	public:
		SubMenu(SubMenu *parent, const char *text) : MenuItem(parent, text) {	};


		// Construct children
		MenuSeparator* 		addSeparator(const char* text);
		SubMenu* 			addSubMenu(const char* text);
		FloatField* 		addFloat(const char* name, float* f);

	protected:
		void draw_btn(lv_obj_t *lv_list);
		void draw_open();
		void draw_close();
		virtual void draw_first_btn(lv_obj_t *lv_list);
		static void close_cb(lv_event_t *e);
	private:
		lv_obj_t *_list = nullptr;
};

class TreeMenu : public SubMenu
{
	public:
		TreeMenu() : SubMenu(nullptr, "Root") { };
		void draw_first_btn(lv_obj_t *lv_list);
};

#endif // __TREEMENU_H