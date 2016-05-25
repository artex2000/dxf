#ifndef __DXF_H__
#define __DXF_H__

typedef struct {
	double x;
	double y;
	double z;
} point_t;

typedef struct {
	int type;
	point_t a;
	point_t b;
} line_t;

typedef struct {
	int type;
	point_t c;
	double r;
	double a1;
	double a2;
} arc_t;

extern list_item contour;

void dxf_parser (char *fname);

#endif
