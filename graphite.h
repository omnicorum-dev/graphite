//
// Created by Nico Russo on 4/14/26.
//

#ifndef GRAPHITE_H
#define GRAPHITE_H

#include "base.h"

namespace Graphite {
    using namespace omni::basic;

    struct Color {
        union {
            uint32_t color{};
            struct { u8 r, g, b, a; };
        };

        Color(u8 r, u8 g, u8 b, u8 a = 0xFF)
            : r(r), g(g), b(b), a(a) {}

        Color(u32 c = 0xFF000000) {
            color = c;
        }

        operator uint32_t() const { return color; }
    };

    class Canvas {
    private:
        u32 *pixels;
        u32 WIDTH;
        u32 HEIGHT;

        u32 xyToIdx(const u32 x, const u32 y) const {
            return y * WIDTH + x;
        };

    public:
        Canvas(const u32 w, const u32 h) {
            pixels = new u32[w * h];
            WIDTH = w;
            HEIGHT = h;
        }
        ~Canvas() {
            delete[] pixels;
        }

        u32 getWidth () const { return WIDTH; }
        u32 getHeight() const { return HEIGHT; }

        void fill(const Color color) const {
            for (u64 i = 0; i < WIDTH * HEIGHT; i++) {
                pixels[i] = color;
            }
        }

        void fillRect(const i32 x0, const i32 y0, const i32 width, const i32 height, const Color color) const {
            for (i32 dy = 0; dy < height; dy++) {
                const i32 y = y0 + dy;
                if (0 <= y && y < HEIGHT) {
                    for (i32 dx = 0; dx < width; dx++) {
                        const i32 x = x0 + dx;
                        if (0 <= x && x < WIDTH) {
                            pixels[xyToIdx(x, y)] = color;
                        }
                    }
                }
            }
        }

        void fillCircle(const i32 cx, const i32 cy, const i32 radius, const Color color) const {
            const i32 x1 = cx - radius;
            const i32 y1 = cy - radius;
            const i32 x2 = cx + radius;
            const i32 y2 = cy + radius;
            for (i32 y = y1; y <= y2; y++) {
                if (0 <= y && y < HEIGHT) {
                    for (i32 x = x1; x <= x2; x++) {
                        if (0 <= x && x < WIDTH) {
                            const int dy = y - cy;
                            const int dx = x - cx;
                            if (dx*dx + dy*dy <= radius*radius) {
                                pixels[xyToIdx(x, y)] = color;
                            }
                        }
                    }
                }
            }
        }

        void drawLine(i32 x1, i32 y1, i32 x2, i32 y2, const Color color) const {
            i32 dx = abs(x2 - x1);
            i32 dy = abs(y2 - y1);

            i32 sx = (x1 < x2) ? 1 : -1;
            i32 sy = (y1 < y2) ? 1 : -1;

            i32 err = dx - dy;

            while (true) {
                if (0 <= x1 && x1 < WIDTH && 0 <= y1 && y1 < HEIGHT) {
                    pixels[xyToIdx(x1, y1)] = color;
                }

                if (x1 == x2 && y1 == y2) break;

                i32 e2 = 2 * err;

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

        bool saveToPPM (const std::string& filename) const {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (file.fail()) {
                LOG_ERROR("Could not open file {}", filename);
                return false;
            }

            println(file, "P6\n{} {} 255", WIDTH, HEIGHT);

            for (u64 i = 0; i < WIDTH * HEIGHT; i++) {
                u32 pixel = pixels[i];
                u8 bytes[3] = {
                    (u8)((pixel >> 0)  & 0xff),
                    (u8)((pixel >> 8)  & 0xff),
                    (u8)((pixel >> 16) & 0xff)
                };
                file << bytes[0] << bytes[1] << bytes[2];
            }

            file.close();
            return true;
        }
    };


}

#endif //GRAPHITE_H
