#ifndef cursor_type_h_INCLUDED
#define cursor_type_h_INCLUDED

// VOLATILE - CURSOR_TYPE_LEN and CursorType length must agree
#define CURSOR_TYPE_LEN 10

enum CursorType {
	CUR_NULL, // used to wrap around in config menus, for instance
	CUR_CLASSIC,
	CUR_VERT,
	CUR_WARP,
	CUR_ROW,
	CUR_COL,
	CUR_SNAKE,
	CUR_RING,
	CUR_TWOBYTWO,
	CUR_HSPLIT
};

#endif // cursor_type_h_INCLUDED
