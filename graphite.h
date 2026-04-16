//
// Created by Nico Russo on 4/14/26.
//

#ifndef GRAPHITE_H
#define GRAPHITE_H

#include <base.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <vendor/font8x8_basic.h>

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

    struct NormalizedRectangle {
        i32 x1, y1, x2, y2;

        i32 ox1, oy1, ox2, oy2;

        bool normalizeRectangle(const i32 x, const i32 y,
                            const i32 w, const i32 h,
                            const i32 canvas_width,
                            const i32 canvas_height)
        {
            if (w == 0 || h == 0)
                return false;

            // ---- STEP 1: compute ORIGINAL bounds (NO CLAMPING) ----
            ox1 = x;
            oy1 = y;
            ox2 = x + w;
            oy2 = y + h;

            if (ox1 > ox2) std::swap(ox1, ox2);
            if (oy1 > oy2) std::swap(oy1, oy2);

            // ---- STEP 2: cull using ORIGINAL bounds ----
            if (ox1 >= canvas_width || ox2 < 0 ||
                oy1 >= canvas_height || oy2 < 0)
                return false;

            // ---- STEP 3: compute CLIPPED bounds ----
            x1 = std::max(0, std::min(ox1, canvas_width - 1));
            y1 = std::max(0, std::min(oy1, canvas_height - 1));
            x2 = std::max(0, std::min(ox2, canvas_width - 1));
            y2 = std::max(0, std::min(oy2, canvas_height - 1));

            // ---- STEP 4: safety check (important) ----
            if (x1 > x2 || y1 > y2)
                return false;

            return true;
        }
    };

    u8 mixComponent(const u16 c1, const u16 c2, const u16 a) {
        return c1 + (c2 - c1)*a/255;
    }

    Color mixColors (const Color c1, const Color c2) {
        return {
            mixComponent(c1.r, c2.r, c2.a),
            mixComponent(c1.g, c2.g, c2.a),
            mixComponent(c1.b, c2.b, c2.a),
            mixComponent(c1.a, c2.a, c2.a),
        };
    }

    class Canvas {
    protected:
        u32 *pixels;
        u32 WIDTH;
        u32 HEIGHT;
        u32 STRIDE;

        bool linked = false;
        unsigned char* rawData = nullptr;

        u32* getPixel(const u32 x, const u32 y) const {
            return &pixels[(y)*STRIDE + (x)];
        }
        Color getPixelColor(const u32 x, const u32 y) const {
            return pixels[(y)*STRIDE + (x)];
        }
        u32 getPixelIndex(const u32 x, const u32 y) const {
            return (y)*STRIDE + (x);
        }

    public:
        Canvas(const u32 width, const u32 height, const u32 stride) {
            pixels = new u32[width * height];
            WIDTH = width;
            HEIGHT = height;
            STRIDE = stride;
        }
        Canvas(const u32 width, const u32 height) {
            pixels = new u32[width * height];
            WIDTH = width;
            HEIGHT = height;
            STRIDE = width;
        }
        Canvas(const std::string spriteFilepath) {
            i32 x;
            i32 y;
            i32 n;
            rawData = stbi_load(spriteFilepath.c_str(), &x, &y, &n, 4);
            if (!rawData) {
                LOG_ERROR("failed to load sprite file {}", spriteFilepath);
                return;
            }
            pixels = reinterpret_cast<u32*>(rawData);
            WIDTH = (u32)x;
            HEIGHT = (u32)y;
            STRIDE = (u32)x;
        }
        Canvas() {
            pixels = nullptr;
            WIDTH = 0;
            HEIGHT = 0;
            STRIDE = 0;
        }
        ~Canvas() {
            if (rawData != nullptr)
                stbi_image_free(rawData);
            else if (!linked)
                delete[] pixels;
        }

        void linkCanvas(u32* pixelLink, const u32 width, const u32 height) {
            linked = true;
            pixels = pixelLink;
            WIDTH = width;
            HEIGHT = height;
            STRIDE = width;
        }
        void newCanvas(const u32 width, const u32 height) {
            delete[] pixels;
            pixels = new u32[width * height];
            WIDTH = width;
            HEIGHT = height;
            STRIDE = width;
        }
        void newCanvas(const u32 width, const u32 height, const u32 stride) {
            delete[] pixels;
            pixels = new u32[width * height];
            WIDTH = width;
            HEIGHT = height;
            STRIDE = stride;
        }

        u32 getWidth () const { return WIDTH; }
        u32 getHeight() const { return HEIGHT; }
        u32 getStride () const { return STRIDE; }
        void setWidth (const u32 width) { WIDTH = width; }
        void setHeight (const u32 height) { HEIGHT = height; }
        void setStride (const u32 stride) { STRIDE = stride; }

        void fill(const Color color) const {
            for (size_t y = 0; y < HEIGHT; ++y) {
                for (size_t x = 0; x < WIDTH; ++x) {
                    pixels[getPixelIndex(x, y)] = color;
                }
            }
        }

        void fillFast(const Color color) const {
            std::fill_n(pixels, WIDTH * HEIGHT, color);
        }

        void fillStupid(const unsigned char c) const {
            memset(pixels, c, sizeof(u32) * WIDTH * HEIGHT);
        }

        void fillRect(const i32 x0, const i32 y0, const i32 width, const i32 height, const Color color) const {
            NormalizedRectangle nr{};
            if (!nr.normalizeRectangle(x0, y0, width, height, WIDTH, HEIGHT)) return;
            for (i32 x = nr.x1; x <= nr.x2; ++x) {
                for (i32 y = nr.y1; y <= nr.y2; ++y) {
#ifdef ALPHA_BLEND
                    pixels[getPixelIndex(x, y)] = mixColors(pixels[getPixelIndex(x, y)], color);
#else
                    pixels[getPixelIndex(x, y)]= color;
#endif
                }
            }
        }

        void drawCanvas(const i32 x0, const i32 y0, const i32 width, const i32 height, Canvas& sourceCanvas) const {
            if (sourceCanvas.getWidth() == 0) return;
            if (sourceCanvas.getHeight() == 0) return;

            NormalizedRectangle nr{};
            if (!nr.normalizeRectangle(x0, y0, width, height, WIDTH, HEIGHT)) return;

            i32 xa = nr.ox1;
            if (width < 0) xa = nr.ox2;
            i32 ya = nr.oy1;
            if (height < 0) ya = nr.oy2;
            for (i32 y = nr.y1; y <= nr.y2; ++y) {
                for (i32 x = nr.x1; x <= nr.x2; ++x) {
                    u32 nx = (x - xa)*((i32)sourceCanvas.getWidth())/width;
                    u32 ny = (y - ya)*((i32)sourceCanvas.getHeight())/height;

                    if (nx >= sourceCanvas.getWidth())  nx = sourceCanvas.getWidth() - 1;
                    if (ny >= sourceCanvas.getHeight()) ny = sourceCanvas.getHeight() - 1;

                    Color sourcePixel = sourceCanvas.getPixelColor(nx, ny);
                    if (sourcePixel.a != 0) {
#ifdef ALPHA_BLEND
                        pixels[getPixelIndex(x, y)] = mixColors(pixels[getPixelIndex(x, y)], sourcePixel);
#else
                        pixels[getPixelIndex(x, y)]= sourcePixel;
#endif
                    }
                }
            }
        }

        void blitCanvas(Canvas& sourceCanvas) const {
            const i32 srcW = (i32)sourceCanvas.getWidth();
            const i32 srcH = (i32)sourceCanvas.getHeight();

            const i32 dstW = (i32)WIDTH;
            const i32 dstH = (i32)HEIGHT;

            const i32 srcStride = sourceCanvas.getStride();
            const i32 dstStride = STRIDE;

            // 16.16 fixed point
            const i32 stepX = (srcW << 16) / dstW;
            const i32 stepY = (srcH << 16) / dstH;

            i32 srcY_fp = 0;

            for (i32 y = 0; y < dstH; ++y) {
                const i32 sy = srcY_fp >> 16;
                const u32* srcRow = sourceCanvas.pixels + sy * srcStride;
                u32* dstRow = pixels + y * dstStride;

                i32 srcX_fp = 0;

                // 🔥 pointer-based inner loop
                u32* d = dstRow;

                for (i32 x = 0; x < dstW; ++x) {
                    const i32 sx = srcX_fp >> 16;
                    *d++ = srcRow[sx];
                    srcX_fp += stepX;
                }

                srcY_fp += stepY;
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
#ifdef ALPHA_BLEND
                                pixels[getPixelIndex(x, y)] = mixColors(pixels[getPixelIndex(x, y)], color);
#else
                                pixels[getPixelIndex(x, y)]= color;
#endif
                            }
                        }
                    }
                }
            }
        }

        void fillEllipse(const i32 cx, const i32 cy, const i32 rx, i32 ry, const Color color) const {
            NormalizedRectangle nr = {0};

            int rx1 = rx;
            int ry1 = ry;

            if (!nr.normalizeRectangle(
                    cx - rx1,
                    cy - ry1,
                    2 * rx1,
                    2 * ry1,
                    WIDTH,
                    HEIGHT))
                return;

            for (i32 y = nr.y1; y <= nr.y2; ++y) {
                for (i32 x = nr.x1; x <= nr.x2; ++x) {

                    float w = (float)(nr.x2 - nr.x1 + 1);
                    float h = (float)(nr.y2 - nr.y1 + 1);

                    float nx = (x + 0.5f - nr.x1) / w;
                    float ny = (y + 0.5f - nr.y1) / h;

                    float dx = nx - 0.5f;
                    float dy = ny - 0.5f;

                    if (dx * dx + dy * dy <= 0.25f) {
#ifdef ALPHA_BLEND
                        pixels[getPixelIndex(x, y)] = mixColors(pixels[getPixelIndex(x, y)], color);
#else
                        pixels[getPixelIndex(x, y)]= color;
#endif
                    }
                }
            }
        }

        void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color color) const {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y) {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            // Bounding box
            const i32 minX = std::max(0, std::min({x1, x2, x3}));
            const i32 maxX = std::min((i32)WIDTH - 1, std::max({x1, x2, x3}));
            const i32 minY = std::max(0, std::min({y1, y2, y3}));
            const i32 maxY = std::min((i32)HEIGHT - 1, std::max({y1, y2, y3}));

            for (i32 y = minY; y <= maxY; y++) {
                for (i32 x = minX; x <= maxX; x++) {

                    const i32 w0 = edge(x2, y2, x3, y3, x, y);
                    const i32 w1 = edge(x3, y3, x1, y1, x, y);
                    const i32 w2 = edge(x1, y1, x2, y2, x, y);

                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
#ifdef ALPHA_BLEND
                        pixels[getPixelIndex(x, y)] = mixColors(pixels[getPixelIndex(x, y)], color);
#else
                        pixels[getPixelIndex(x, y)]= color;
#endif
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
#ifdef ALPHA_BLEND
                    pixels[getPixelIndex(x1, y1)] = mixColors(pixels[getPixelIndex(x1, y1)], color);
#else
                    pixels[getPixelIndex(x1, y1)]= color;
#endif
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

        void writeChar(char c, i32 x, i32 y, int font_size, int color) {
            const uint8_t* glyph = font8x8_basic[(uint8_t)c];

            for (int py = 0; py < font_size; ++py) {
                // Map output y to glyph row (0..7)
                int glyph_row = (py * 8) / font_size;

                uint8_t bits = glyph[glyph_row];

                for (int px = 0; px < font_size; ++px) {
                    // Map output x to glyph column (0..7)
                    int glyph_col = (px * 8) / font_size;

                    if (bits & (1 << glyph_col)) {
                        //set_pixel_color(x + px, y + py, color);
                        int draw_x = x + px;
                        int draw_y = y - font_size + py;
                        pixels[getPixelIndex(draw_x, draw_y)] = color;
                    }
                }
            }
        }

        void writeString(const std::string s, i32 x, i32 y, int font_size, int color) {
            int cursor_x = x;
            int cursor_y = y;

            for (int i = 0; s[i] != '\0'; ++i) {
                char c = s[i];
                writeChar(c, cursor_x, cursor_y, font_size, color);
                cursor_x += font_size;
            }
        }

        Canvas subcanvas(const i32 x, const i32 y, const i32 w, const i32 h) const {
            NormalizedRectangle nr{};
            Canvas c(0, 0);
            if (nr.normalizeRectangle(x, y, w, h, WIDTH, HEIGHT)) return c;
            c.pixels = getPixel(x, y);
            c.setWidth(nr.x2 - nr.x1 + 1);
            c.setHeight(nr.y2 - nr.y1 + 1);
            return c;
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
