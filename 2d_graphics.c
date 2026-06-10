#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 60
#define HEIGHT 25
#define MAX_OBJECTS 100

/* ---------- Shape Types ---------- */
typedef enum {
    LINE,
    RECTANGLE,
    CIRCLE,
    TRIANGLE
} ShapeType;

/* ---------- Shape Structure ---------- */
typedef struct {
    ShapeType type;

    union {
        struct {
            int x1, y1, x2, y2;
        } line;

        struct {
            int x, y, w, h;
        } rect;

        struct {
            int xc, yc, r;
        } circle;

        struct {
            int x1, y1, x2, y2, x3, y3;
        } triangle;
    };
} Shape;

/* ---------- Global Data ---------- */
char canvas[HEIGHT][WIDTH];
Shape objects[MAX_OBJECTS];
int objectCount = 0;

/* ---------- Canvas Functions ---------- */

void clearCanvas() {
    for (int i = 0; i < HEIGHT; i++)
        for (int j = 0; j < WIDTH; j++)
            canvas[i][j] = ' ';
}

void setPixel(int x, int y) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
        canvas[y][x] = '*';
}

void displayCanvas() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++)
            printf("%c", canvas[i][j]);
        printf("\n");
    }
}

/* ---------- Drawing Functions ---------- */

/* Bresenham Line */
void drawLine(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while (1) {
        setPixel(x1, y1);

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void drawRectangle(int x, int y, int w, int h) {
    for (int i = x; i <= x + w; i++) {
        setPixel(i, y);
        setPixel(i, y + h);
    }

    for (int j = y; j <= y + h; j++) {
        setPixel(x, j);
        setPixel(x + w, j);
    }
}

void drawCircle(int xc, int yc, int r) {
    for (int angle = 0; angle < 360; angle++) {
        double rad = angle * 3.14159265 / 180.0;

        int x = xc + (int)(r * cos(rad));
        int y = yc + (int)(r * sin(rad));

        setPixel(x, y);
    }
}

void drawTriangle(int x1, int y1,
                  int x2, int y2,
                  int x3, int y3) {
    drawLine(x1, y1, x2, y2);
    drawLine(x2, y2, x3, y3);
    drawLine(x3, y3, x1, y1);
}

/* ---------- Object Management ---------- */

void addObject(Shape s) {
    if (objectCount < MAX_OBJECTS)
        objects[objectCount++] = s;
}

void deleteObject(int index) {
    if (index < 0 || index >= objectCount)
        return;

    for (int i = index; i < objectCount - 1; i++)
        objects[i] = objects[i + 1];

    objectCount--;
}

void modifyObject(int index, Shape newShape) {
    if (index >= 0 && index < objectCount)
        objects[index] = newShape;
}

/* ---------- Redraw Scene ---------- */

void redrawScene() {
    clearCanvas();

    for (int i = 0; i < objectCount; i++) {

        switch (objects[i].type) {

        case LINE:
            drawLine(
                objects[i].line.x1,
                objects[i].line.y1,
                objects[i].line.x2,
                objects[i].line.y2
            );
            break;

        case RECTANGLE:
            drawRectangle(
                objects[i].rect.x,
                objects[i].rect.y,
                objects[i].rect.w,
                objects[i].rect.h
            );
            break;

        case CIRCLE:
            drawCircle(
                objects[i].circle.xc,
                objects[i].circle.yc,
                objects[i].circle.r
            );
            break;

        case TRIANGLE:
            drawTriangle(
                objects[i].triangle.x1,
                objects[i].triangle.y1,
                objects[i].triangle.x2,
                objects[i].triangle.y2,
                objects[i].triangle.x3,
                objects[i].triangle.y3
            );
            break;
        }
    }
}

/* ---------- Main ---------- */

int main() {

    Shape s;

    /* Add Rectangle */
    s.type = RECTANGLE;
    s.rect.x = 2;
    s.rect.y = 2;
    s.rect.w = 15;
    s.rect.h = 6;
    addObject(s);

    /* Add Circle */
    s.type = CIRCLE;
    s.circle.xc = 35;
    s.circle.yc = 10;
    s.circle.r = 5;
    addObject(s);

    /* Add Line */
    s.type = LINE;
    s.line.x1 = 0;
    s.line.y1 = 0;
    s.line.x2 = 20;
    s.line.y2 = 15;
    addObject(s);

    /* Add Triangle */
    s.type = TRIANGLE;
    s.triangle.x1 = 45;
    s.triangle.y1 = 3;
    s.triangle.x2 = 55;
    s.triangle.y2 = 15;
    s.triangle.x3 = 35;
    s.triangle.y3 = 15;
    addObject(s);

    printf("INITIAL PICTURE\n\n");
    redrawScene();
    displayCanvas();

    printf("\nDELETE OBJECT 2 (LINE)\n\n");
    deleteObject(2);
    redrawScene();
    displayCanvas();

    printf("\nMODIFY RECTANGLE\n\n");

    Shape newRect;
    newRect.type = RECTANGLE;
    newRect.rect.x = 5;
    newRect.rect.y = 5;
    newRect.rect.w = 20;
    newRect.rect.h = 8;

    modifyObject(0, newRect);

    redrawScene();
    displayCanvas();

    return 0;
}
