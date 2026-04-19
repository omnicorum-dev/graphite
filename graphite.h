//
// Created by Nico Russo on 4/14/26.
//

#ifndef GRAPHITE_H
#define GRAPHITE_H

#include <base.h>

#ifndef NO_STB_IMPL
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#ifndef NO_STB_IMPL
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb_image_write.h>

#include <font8x8_basic.h>

#include <random>
#include <cstdint>

inline uint32_t randomUint32()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

    return dist(gen);
}

namespace Graphite {
    using namespace omni::basic;

    struct Color {
        union {
            uint32_t color{};
            struct { u8 r, g, b, a; };
        };

        constexpr Color(u8 r, u8 g, u8 b, u8 a = 0xFF)
            : r(r), g(g), b(b), a(a) {}

        constexpr Color(const u32 c = 0xFF000000) {
            color = c;
        }

        operator uint32_t() const { return color; }
    };

    namespace Colors {
        inline constexpr Color Red       {0xff, 0x50, 0x50};
        inline constexpr Color Green     {0x50, 0xcc, 0x50};
        inline constexpr Color Blue      {0x50, 0x50, 0xff};
        inline constexpr Color White     {0xff, 0xff, 0xff};
        inline constexpr Color Black     {0x00, 0x00, 0x00};
        inline constexpr Color Yellow    {0xff, 0xff, 0x50};
        inline constexpr Color Pink      {0xff, 0x50, 0xff};
        inline constexpr Color Orange    {0xff, 0x99, 0x50};
        inline constexpr Color Cyan      {0x50, 0xcc, 0xff};
        inline constexpr Color Purple    {0xdd, 0x50, 0xee};
        inline constexpr Color Tan       {0xcc, 0xee, 0xaa};
        inline constexpr Color DarkGrey  {0x18, 0x18, 0x18};
        inline constexpr Color LightGrey {0xcc, 0xcc, 0xcc};
        inline constexpr Color Brown     {0x8B, 0x45, 0x13};
    }

    // h = 0..360, s = 0..1, l = 0..1
    constexpr Color HSLtoRGB(float hueAngleDeg, float satNorm, float lightNorm, const u8 alpha = 255) {
        // h = 0..360
        // s = 0..1
        // l = 0..1

        hueAngleDeg = std::fmod(hueAngleDeg, 360.0f);
        if (hueAngleDeg < 0.0f) hueAngleDeg += 360.0f;

        satNorm = std::clamp(satNorm, 0.0f, 1.0f);
        lightNorm = std::clamp(lightNorm, 0.0f, 1.0f);

        const float c = (1.0f - std::fabs(2.0f * lightNorm - 1.0f)) * satNorm;
        const float x = c * (1.0f - std::fabs(std::fmod(hueAngleDeg / 60.0f, 2.0f) - 1.0f));
        const float m = lightNorm - c * 0.5f;

        float r1 = 0, g1 = 0, b1 = 0;

        if      (hueAngleDeg < 60)  { r1 = c; g1 = x; b1 = 0; }
        else if (hueAngleDeg < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (hueAngleDeg < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (hueAngleDeg < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (hueAngleDeg < 300) { r1 = x; g1 = 0; b1 = c; }
        else              { r1 = c; g1 = 0; b1 = x; }

        u8 r = static_cast<uint8_t>((r1 + m) * 255.0f + 0.5f);
        u8 g = static_cast<uint8_t>((g1 + m) * 255.0f + 0.5f);
        u8 b = static_cast<uint8_t>((b1 + m) * 255.0f + 0.5f);

        return Color(r, g, b, alpha);
    }
    // h = 0..360, s = 0..1, v = 0..1
    constexpr Color HSVtoRGB(float hueAngleDeg, float satNorm, float valueNorm, const u8 alpha = 255) {
        // h = 0..360
        // s = 0..1
        // v = 0..1

        hueAngleDeg = std::fmod(hueAngleDeg, 360.0f);
        if (hueAngleDeg < 0.0f) hueAngleDeg += 360.0f;

        satNorm = std::clamp(satNorm, 0.0f, 1.0f);
        valueNorm = std::clamp(valueNorm, 0.0f, 1.0f);

        const float c = valueNorm * satNorm;
        const float x = c * (1.0f - std::fabs(std::fmod(hueAngleDeg / 60.0f, 2.0f) - 1.0f));
        const float m = valueNorm - c;

        float r1 = 0, g1 = 0, b1 = 0;

        if      (hueAngleDeg < 60)  { r1 = c; g1 = x; b1 = 0; }
        else if (hueAngleDeg < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (hueAngleDeg < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (hueAngleDeg < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (hueAngleDeg < 300) { r1 = x; g1 = 0; b1 = c; }
        else              { r1 = c; g1 = 0; b1 = x; }

        const u8 r = static_cast<uint8_t>((r1 + m) * 255.0f + 0.5f);
        const u8 g = static_cast<uint8_t>((g1 + m) * 255.0f + 0.5f);
        const u8 b = static_cast<uint8_t>((b1 + m) * 255.0f + 0.5f);

        return Color(r, g, b, alpha);
    }

    inline u8 mixComponent(const u16 c1, const u16 c2, const u16 a) {
        return c1 + (c2 - c1)*a/255;
    }
    inline Color mixColors (const Color c1, const Color c2) {
        return {
            mixComponent(c1.r, c2.r, c2.a),
            mixComponent(c1.g, c2.g, c2.a),
            mixComponent(c1.b, c2.b, c2.a),
            mixComponent(c1.a, c2.a, c2.a),
        };
    }
    inline Color mixColorsTri(const Color c1, const Color c2, const Color c3, const float w0, const float w1, const float w2) {
        return {
            static_cast<u8>(c1.r * w0 + c2.r * w1 + c3.r * w2),
            static_cast<u8>(c1.g * w0 + c2.g * w1 + c3.g * w2),
            static_cast<u8>(c1.b * w0 + c2.b * w1 + c3.b * w2),
            static_cast<u8>(c1.a * w0 + c2.a * w1 + c3.a * w2)
        };
    }
    inline f32 mixDepths(const f32 c1, const f32 c2, const f32 c3, const float w0, const float w1, const float w2) {
        return c1 * w0 + c2 * w1 + c3 * w2;
    }

    inline omni::Vec2<f32> colorToUV(const Color& color) {
        return {color.r/255.0f, color.g/255.0f};
    }

    struct NormalizedRectangle {
        i32 x1, y1, x2, y2;
        i32 ox1, oy1, ox2, oy2;

        bool normalizeRectangle(const i32 x, const i32 y, const i32 w, const i32 h, const i32 canvas_width, const i32 canvas_height) {
            if (w == 0 || h == 0)
                return false;

            // ---- STEP 1: compute ORIGINAL bounds (NO CLAMPING) ----
            ox1 = x; oy1 = y; ox2 = x + w; oy2 = y + h;

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

    class Canvas {
    public:
        // ====================== CONSTRUCTORS ======================
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
        Canvas(const std::string& spriteFilepath) {
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

        // ====================== INITIALIZERS ======================
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

        // ==================== GETTERS/SETTERS =====================
        u32 getWidth () const { return WIDTH; }
        u32 getHeight() const { return HEIGHT; }
        u32 getStride () const { return STRIDE; }
        void setWidth (const u32 width) { WIDTH = width; }
        void setHeight (const u32 height) { HEIGHT = height; }
        void setStride (const u32 stride) { STRIDE = stride; }

        // ===================== SUB-CANVASING ======================
        Canvas subcanvas(const i32 x, const i32 y, const i32 w, const i32 h) const {
            NormalizedRectangle nr{};
            Canvas c(0, 0);
            if (nr.normalizeRectangle(x, y, w, h, WIDTH, HEIGHT)) return c;
            c.pixels = getPixel(x, y);
            c.setWidth(nr.x2 - nr.x1 + 1);
            c.setHeight(nr.y2 - nr.y1 + 1);
            return c;
        }
        Canvas subcanvas(const omni::Vec2<i32> p, const i32 w, const i32 h) const {
            NormalizedRectangle nr{};
            Canvas c(0, 0);
            if (nr.normalizeRectangle(p.x, p.y, w, h, WIDTH, HEIGHT)) return c;
            c.pixels = getPixel(p.x, p.y);
            c.setWidth(nr.x2 - nr.x1 + 1);
            c.setHeight(nr.y2 - nr.y1 + 1);
            return c;
        }

        // ==================== FULL-CANVAS FILL ====================
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

        // ======================= DRAWING ==========================
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
        void drawCanvas(const omni::Vec2<i32> p, const i32 width, const i32 height, Canvas& sourceCanvas) const {
            drawCanvas(p.x, p.y, width, height, sourceCanvas);
        }

        void blitCanvas(const Canvas& sourceCanvas) const {
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
        void fillRect(const omni::Vec2<i32>& p, const i32 width, const i32 height, const Color color) const {
            fillRect(p.x, p.y, width, height, color);
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
        void fillCircle(const omni::Vec2<i32>& p, const i32 radius, const Color color) const {
            fillCircle(p.x, p.y, radius, color);
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
        void fillEllipse(const omni::Vec2<i32>& p, const i32 rx, const i32 ry, const Color color) const {
            fillEllipse(p.x, p.y, rx, ry, color);
        }
        void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color color) const {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y) {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            // signed area
            const int area = edge(x1, y1, x2, y2, x3, y3);
            if (area == 0) return;

            const float invArea = 1.0f / (float)area;

            // Bounding box
            const int minX = std::max(0, std::min({x1, x2, x3}));
            const int maxX = std::min((int)WIDTH - 1, std::max({x1, x2, x3}));
            const int minY = std::max(0, std::min({y1, y2, y3}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({y1, y2, y3}));

            if (minX > maxX || minY > maxY) return;

            // Edge deltas
            const int dx01 = x2 - x1;
            const int dy01 = y2 - y1;

            const int dx12 = x3 - x2;
            const int dy12 = y3 - y2;

            const int dx20 = x1 - x3;
            const int dy20 = y1 - y3;

            // Evaluate once at top-left pixel of bbox
            int w0_row = edge(x2, y2, x3, y3, minX, minY);
            int w1_row = edge(x3, y3, x1, y1, minX, minY);
            int w2_row = edge(x1, y1, x2, y2, minX, minY);

            for (int y = minY; y <= maxY; ++y)
            {
                int w0 = w0_row;
                int w1 = w1_row;
                int w2 = w2_row;

                u32* row = pixels + y * WIDTH;

                for (int x = minX; x <= maxX; ++x)
                {
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        const float b0 = w0 * invArea;
                        const float b1 = w1 * invArea;
                        const float b2 = w2 * invArea;
#ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], color);
#else
                        row[x] = color;
#endif
                    }

                    // move right one pixel
                    w0 += dy12;
                    w1 += dy20;
                    w2 += dy01;
                }

                // move down one row
                w0_row -= dx12;
                w1_row -= dx20;
                w2_row -= dx01;
            }
        }
        void fillTriangle(const omni::Vec2<i32>&p1, const omni::Vec2<i32>&p2, const omni::Vec2<i32>&p3, const Color color) const {
            fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
        }
        void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color c1, const Color c2, const Color c3) const {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y) {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            // signed area
            const int area = edge(x1, y1, x2, y2, x3, y3);
            if (area == 0) return;

            const float invArea = 1.0f / (float)area;

            // Bounding box
            const int minX = std::max(0, std::min({x1, x2, x3}));
            const int maxX = std::min((int)WIDTH - 1, std::max({x1, x2, x3}));
            const int minY = std::max(0, std::min({y1, y2, y3}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({y1, y2, y3}));

            if (minX > maxX || minY > maxY) return;

            // Edge deltas
            const int dx01 = x2 - x1;
            const int dy01 = y2 - y1;

            const int dx12 = x3 - x2;
            const int dy12 = y3 - y2;

            const int dx20 = x1 - x3;
            const int dy20 = y1 - y3;

            // Evaluate once at top-left pixel of bbox
            int w0_row = edge(x2, y2, x3, y3, minX, minY);
            int w1_row = edge(x3, y3, x1, y1, minX, minY);
            int w2_row = edge(x1, y1, x2, y2, minX, minY);

            for (int y = minY; y <= maxY; ++y)
            {
                int w0 = w0_row;
                int w1 = w1_row;
                int w2 = w2_row;

                u32* row = pixels + y * WIDTH;

                for (int x = minX; x <= maxX; ++x)
                {
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        const float b0 = w0 * invArea;
                        const float b1 = w1 * invArea;
                        const float b2 = w2 * invArea;

                        const Color c = mixColorsTri(c1, c2, c3, b0, b1, b2);

#ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], c);
#else
                        row[x] = c;
#endif
                    }

                    // move right one pixel
                    w0 += dy12;
                    w1 += dy20;
                    w2 += dy01;
                }

                // move down one row
                w0_row -= dx12;
                w1_row -= dx20;
                w2_row -= dx01;
            }
        }
        void fillTriangle( const omni::Vec2<i32>&p1, const omni::Vec2<i32>&p2, const omni::Vec2<i32>&p3, const Color c1, const Color c2, const Color c3) const {
            fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, c1, c2, c3);
        }

        void fillTriangleZ(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color c1, const Color c2, const Color c3, const f32 invZ1, const f32 invZ2, const f32 invZ3, const Canvas& zBuffer) const {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y) {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            // signed area
            const int area = edge(x1, y1, x2, y2, x3, y3);
            if (area == 0) return;

            const float invArea = 1.0f / (float)area;

            // Bounding box
            const int minX = std::max(0, std::min({x1, x2, x3}));
            const int maxX = std::min((int)WIDTH - 1, std::max({x1, x2, x3}));
            const int minY = std::max(0, std::min({y1, y2, y3}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({y1, y2, y3}));

            if (minX > maxX || minY > maxY) return;

            // Edge deltas
            const int dx01 = x2 - x1;
            const int dy01 = y2 - y1;

            const int dx12 = x3 - x2;
            const int dy12 = y3 - y2;

            const int dx20 = x1 - x3;
            const int dy20 = y1 - y3;

            // Evaluate once at top-left pixel of bbox
            int w0_row = edge(x2, y2, x3, y3, minX, minY);
            int w1_row = edge(x3, y3, x1, y1, minX, minY);
            int w2_row = edge(x1, y1, x2, y2, minX, minY);

            for (int y = minY; y <= maxY; ++y)
            {
                int w0 = w0_row;
                int w1 = w1_row;
                int w2 = w2_row;

                u32* row = pixels + y * WIDTH;
                u32* zRow = zBuffer.pixels + y * WIDTH;

                for (int x = minX; x <= maxX; ++x)
                {
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        const float b0 = w0 * invArea;
                        const float b1 = w1 * invArea;
                        const float b2 = w2 * invArea;

                        const Color c = mixColorsTri(c1, c2, c3, b0, b1, b2);
                        const f32 z = mixDepths(invZ1, invZ2, invZ3, b0, b1, b2);
                        u32 zValue = static_cast<u32>(std::clamp(z, 0.f, 1.f)*UINT16_MAX);
                        zValue |= 0xff000000;

                        if (zValue < zRow[x]) {
                            continue;
                        }
#ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], c);
#else
                        row[x] = c;
#endif
                        zRow[x] = zValue;
                    }

                    // move right one pixel
                    w0 += dy12;
                    w1 += dy20;
                    w2 += dy01;
                }

                // move down one row
                w0_row -= dx12;
                w1_row -= dx20;
                w2_row -= dx01;
            }
        }
        void fillTriangleZ( const omni::Vec2<i32>&p1, const omni::Vec2<i32>&p2, const omni::Vec2<i32>&p3, const Color c1, const Color c2, const Color c3, const f32 invZ1, const f32 invZ2, const f32 invZ3, const Canvas& z) const {
            fillTriangleZ(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, c1, c2, c3, invZ1, invZ2, invZ3, z);
        }

        static Color sampleUV(const omni::Vec2<f32> uv, const Canvas& tex)
        {
            f32 u = std::clamp(uv.x, 0.001f, 0.999f);
            f32 v = std::clamp(1-uv.y, 0.001f, 0.999f);

            return tex.getPixelColor(
                u * tex.getWidth(),
                v * tex.getHeight()
            );
        }

        void fillTriangleUV3D(
            const omni::Vec2<i32>& p0,
            const omni::Vec2<i32>& p1,
            const omni::Vec2<i32>& p2,
            const omni::Vec2<f32>& uvOverZ0,
            const omni::Vec2<f32>& uvOverZ1,
            const omni::Vec2<f32>& uvOverZ2,
            const f32 invZ0,
            const f32 invZ1,
            const f32 invZ2,
            const Canvas& tex) const {
            auto edge = [](const omni::Vec2<i32>& a,
                           const omni::Vec2<i32>& b,
                           const omni::Vec2<i32>& c)
            {
                return (c.x - a.x) * (b.y - a.y)
                     - (c.y - a.y) * (b.x - a.x);
            };

            int area = edge(p0, p1, p2);
            if (area == 0) return;

            float invArea = 1.0f / area;

            const int minX = std::max(0, std::min({p0.x, p1.x, p2.x}));
            const int maxX = std::min((int)WIDTH - 1, std::max({p0.x, p1.x, p2.x}));
            const int minY = std::max(0, std::min({p0.y, p1.y, p2.y}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({p0.y, p1.y, p2.y}));

            if (minX > maxX || minY > maxY) return;

            for (int y = minY; y <= maxY; y++)
            {
                for (int x = minX; x <= maxX; x++)
                {
                    omni::Vec2<i32> p{x, y};

                    int w0 = edge(p1, p2, p);
                    int w1 = edge(p2, p0, p);
                    int w2 = edge(p0, p1, p);

                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        float a = w0 * invArea;
                        float b = w1 * invArea;
                        float c = w2 * invArea;

                        omni::Vec2<f32> uvOverZ =
                            uvOverZ0 * a +
                            uvOverZ1 * b +
                            uvOverZ2 * c;

                        float invZ =
                            invZ0 * a +
                            invZ1 * b +
                            invZ2 * c;

                        omni::Vec2<f32> uv = uvOverZ / invZ;

                        Color col = sampleUV(uv, tex);
                        pixels[y * WIDTH + x] = col;
                    }
                }
            }
        }

        void fillTriangleUV(const omni::Vec2<i32>& p0, const omni::Vec2<i32>& p1, const omni::Vec2<i32>& p2, const omni::Vec2<f32>& uv0, const omni::Vec2<f32>& uv1, const omni::Vec2<f32>& uv2, const Canvas& tex) const {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y) {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            // signed area
            const int area = edge(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
            if (area == 0) return;

            const float invArea = 1.0f / static_cast<float>(area);

            // Bounding box
            const int minX = std::max(0, std::min({p0.x, p1.x, p2.x}));
            const int maxX = std::min((int)WIDTH - 1, std::max({p0.x, p1.x, p2.x}));
            const int minY = std::max(0, std::min({p0.y, p1.y, p2.y}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({p0.y, p1.y, p2.y}));

            if (minX > maxX || minY > maxY) return;

            // Edge deltas
            const int dx01 = p1.x - p0.x;
            const int dy01 = p1.y - p0.y;

            const int dx12 = p2.x - p1.x;
            const int dy12 = p2.y - p1.y;

            const int dx20 = p0.x - p2.x;
            const int dy20 = p0.y - p2.y;

            // Evaluate once at top-left pixel of bbox
            int w0_row = edge(p1.x, p1.y, p2.x, p2.y, minX, minY);
            int w1_row = edge(p2.x, p2.y, p0.x, p0.y, minX, minY);
            int w2_row = edge(p0.x, p0.y, p1.x, p1.y, minX, minY);

            for (int y = minY; y <= maxY; ++y)
            {
                int w0 = w0_row;
                int w1 = w1_row;
                int w2 = w2_row;

                u32* row = pixels + y * WIDTH;

                for (int x = minX; x <= maxX; ++x)
                {
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        const float b0 = w0 * invArea;
                        const float b1 = w1 * invArea;
                        const float b2 = w2 * invArea;

                        const Color c = mixColorsTri(sampleUV(uv0, tex), sampleUV(uv1, tex), sampleUV(uv2, tex), b0, b1, b2);

#ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], c);
#else
                        row[x] = c;
#endif
                    }

                    // move right one pixel
                    w0 += dy12;
                    w1 += dy20;
                    w2 += dy01;
                }

                // move down one row
                w0_row -= dx12;
                w1_row -= dx20;
                w2_row -= dx01;
            }
        }
        void fillTriangleUV(const omni::Vec2<i32>& p0, const omni::Vec2<i32>& p1, const omni::Vec2<i32>& p2, const Color& uv0, const Color& uv1, const Color& uv2, const Canvas& tex) const {
            fillTriangleUV(p0, p1, p2, colorToUV(uv0), colorToUV(uv1), colorToUV(uv2), tex);
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
        void drawLine(const omni::Vec2<i32>& p1, const omni::Vec2<i32>& p2, const Color color) const {
            drawLine(p1.x, p1.y, p2.x, p2.y, color);
        }
        static float distToSegment(float px, float py, float x1, float y1, float x2, float y2) {
            float vx = x2 - x1;
            float vy = y2 - y1;

            float wx = px - x1;
            float wy = py - y1;

            float c1 = wx * vx + wy * vy;
            if (c1 <= 0) return std::sqrt(wx * wx + wy * wy);

            float c2 = vx * vx + vy * vy;
            if (c2 <= c1) {
                float dx = px - x2;
                float dy = py - y2;
                return std::sqrt(dx * dx + dy * dy);
            }

            float t = c1 / c2;

            float bx = x1 + t * vx;
            float by = y1 + t * vy;

            float dx = px - bx;
            float dy = py - by;

            return std::sqrt(dx * dx + dy * dy);
        }
        void drawLine(i32 x1, i32 y1, i32 x2, i32 y2, const Color color, float thickness) const {

            float r = thickness * 0.5f;

            int minX = std::min(x1, x2) - thickness;
            int maxX = std::max(x1, x2) + thickness;
            int minY = std::min(y1, y2) - thickness;
            int maxY = std::max(y1, y2) + thickness;

            minX = std::max(minX, 0);
            minY = std::max(minY, 0);
            maxX = std::min(maxX, (int)WIDTH - 1);
            maxY = std::min(maxY, (int)HEIGHT - 1);

            for (int y = minY; y <= maxY; ++y) {
                for (int x = minX; x <= maxX; ++x) {

                    float d = distToSegment(
                        (float)x, (float)y,
                        (float)x1, (float)y1,
                        (float)x2, (float)y2
                    );

                    if (d <= r) {
#ifdef ALPHA_BLEND
                        pixels[getPixelIndex(x, y)] =
                            mixColors(pixels[getPixelIndex(x, y)], color);
#else
                        pixels[getPixelIndex(x, y)] = color;
#endif
                    }
                }
            }
        }
        void drawLine(const omni::Vec2<i32>& p1, const omni::Vec2<i32>& p2, const Color color, float thickness) const {
            drawLine(p1.x, p1.y, p2.x, p2.y, color, thickness);
        }
        void drawPoint(const i32 x, const i32 y, const i32 radius, const Color color) const {
            i32 rx2 = radius * 2;
            fillRect(x - radius, y - radius, rx2, rx2, color);
        }
        void drawPoint(const omni::Vec2<i32>& p, const i32 radius, const Color color) const {
            drawPoint(p.x, p.y, radius, color);
        }

        void writeCharBaseline(char c, i32 x, i32 y, int font_size, int color) const {
            const uint8_t* glyph = font8x8_basic[(uint8_t)c];

            for (int py = 0; py < font_size; ++py) {
                // Map output y to glyph row (0..7)
                int glyph_row = (py * 8) / font_size;

                uint8_t bits = glyph[glyph_row];

                for (int px = 0; px < font_size; ++px) {
                    int draw_x = x + px;
                    int draw_y = y - font_size + py;
                    if (draw_x < 0 || draw_x >= WIDTH || draw_y < 0 || draw_y >= HEIGHT) continue;

                    // Map output x to glyph column (0..7)
                    int glyph_col = (px * 8) / font_size;

                    if (bits & (1 << glyph_col)) {
                        //set_pixel_color(x + px, y + py, color);

                        pixels[getPixelIndex(draw_x, draw_y)] = color;
                    }
                }
            }
        }
        void writeCharBaseline(char c, const omni::Vec2<i32>& p, int font_size, int color) const {
            writeCharBaseline(c, p.x, p.y, font_size, color);
        }
        void writeStringBaseline(const std::string s, i32 x, i32 y, int font_size, int color) const {
            int cursor_x = x;
            int cursor_y = y;

            for (int i = 0; s[i] != '\0'; ++i) {
                char c = s[i];
                writeCharBaseline(c, cursor_x, cursor_y, font_size, color);
                cursor_x += font_size;
            }
        }
        void writeStringBaseline(const std::string s, const omni::Vec2<i32>& p, int font_size, int color) const {
            writeStringBaseline(s, p.x, p.y, font_size, color);
        }

        // ====================== FILE OUTPUT =======================
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
        bool saveToJPG (const std::string& filename, const int quality = 100) const {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (file.fail()) {
                LOG_ERROR("Could not open file {}", filename);
                return false;
            }

            if (stbi_write_jpg(filename.c_str(), WIDTH, HEIGHT, 4, pixels, quality) != 1) {
                LOG_ERROR("Could not write to file {}", filename);
                file.close();
                return false;
            }

            file.close();
            return true;
        }
        bool saveToPNG (const std::string& filename) const {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (file.fail()) {
                LOG_ERROR("Could not open file {}", filename);
                return false;
            }

            if (stbi_write_png(filename.c_str(), WIDTH, HEIGHT, 4, pixels, sizeof(pixels[0])*WIDTH) != 1) {
                LOG_ERROR("Could not write to file {}", filename);
                file.close();
                return false;
            }

            file.close();
            return true;
        }
        bool saveToBMP (const std::string& filename) const {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (file.fail()) {
                LOG_ERROR("Could not open file {}", filename);
                return false;
            }

            if (stbi_write_bmp(filename.c_str(), WIDTH, HEIGHT, 4, pixels) != 1) {
                LOG_ERROR("Could not write to file {}", filename);
                file.close();
                return false;
            }

            file.close();
            return true;
        }

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
    };

    // =============== 3D ===============
    inline omni::Vec2<i32> normalizedToScreen(const omni::Vec2<f32>& p, const u32 WIDTH, const u32 HEIGHT) {
        const f32 w = static_cast<f32>(WIDTH);
        const f32 h = static_cast<f32>(HEIGHT);
        const f32 s = std::min(w, h);

        const f32 cx = w  * 0.5f;
        const f32 cy = h * 0.5f;

        return {
            static_cast<i32>(cx + p.x * s * 0.5f),
            static_cast<i32>(cy - p.y * s * 0.5f)
        };
    }
    inline omni::Vec2<f32> project(const omni::Vec3<f32>& p) { return {p.x/p.z, p.y/p.z }; }
    inline omni::Vec3<f32> translate(const omni::Vec3<f32>& p, const omni::Vec3<f32>& translation) {
        return {
            p.x + translation.x,
            p.y + translation.y,
            p.z + translation.z
        };
    }
    // rotation: {pitch, yaw, roll} aka {around x, around y, around z}
    inline omni::Vec3<f32> rotate(const omni::Vec3<f32>& p, const omni::Vec3<f32>& rotation) {
        omni::Vec3<f32> r = p;
        // Rotate around X axis
        {
            const f32 c = cosf(rotation.x);
            const f32 s = sinf(rotation.x);

            r = {
                r.x,
                r.y * c - r.z * s,
                r.y * s + r.z * c
            };
        }
        // Rotate around Y axis
        {
            const f32 c = cosf(rotation.y);
            const f32 s = sinf(rotation.y);

            r = {
                r.x * c - r.z * s,
                r.y,
                r.x * s + r.z * c
            };
        }
        // Rotate around Z axis
        {
            const f32 c = cosf(rotation.z);
            const f32 s = sinf(rotation.z);

            r = {
                r.x * c - r.y * s,
                r.x * s + r.y * c,
                r.z
            };
        }

        return r;
    }
    inline omni::Vec3<f32> rotateInverse(const omni::Vec3<f32>& p, const omni::Vec3<f32>& rotation) {
        omni::Vec3<f32> r = p;
        // Rotate around Z axis
        {
            const f32 c = cosf(-rotation.z);
            const f32 s = sinf(-rotation.z);

            r = {
                r.x * c - r.y * s,
                r.x * s + r.y * c,
                r.z
            };
        }
        // Rotate around Y axis
        {
            const f32 c = cosf(-rotation.y);
            const f32 s = sinf(-rotation.y);

            r = {
                r.x * c - r.z * s,
                r.y,
                r.x * s + r.z * c
            };
        }
        // Rotate around X axis
        {
            const f32 c = cosf(-rotation.x);
            const f32 s = sinf(-rotation.x);

            r = {
                r.x,
                r.y * c - r.z * s,
                r.y * s + r.z * c
            };
        }

        return r;
    }

    namespace Dir3D {
        inline const omni::Vec3<f32> UP       {0,  1,  0};
        inline const omni::Vec3<f32> DOWN     {0, -1,  0};
        inline const omni::Vec3<f32> RIGHT    {1,  0,  0};
        inline const omni::Vec3<f32> LEFT     {-1, 0,  0};
        inline const omni::Vec3<f32> FORWARD  {0,  0,  1};
        inline const omni::Vec3<f32> BACKWARD {0,  0,  -1};
    }

    struct Point3D {
        omni::Vec3<f32> position;
        Color color;
    };

    struct Object3D {
        std::vector<Point3D> vertices;
        std::vector<std::vector<i32>> faces;
        omni::Vec3<f32> position = {};
        omni::Vec3<f32> rotation = {};
        Canvas* tex = nullptr;

        void drawObject_UV(const Canvas& c) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
                omni::Vec2<f32> uv;
            };

            std::vector<Vtx> vtx(vertices.size());

            // -----------------------------
            // STEP 1: vertex transform
            // -----------------------------
            for (size_t i = 0; i < vertices.size(); i++)
            {
                omni::Vec3<f32> v = vertices[i].position;

                v = rotate(v, rotation);
                v = translate(v, position);

                omni::Vec2<f32> proj = project(v);
                vtx[i].screen = normalizedToScreen(proj, c.getWidth(), c.getHeight());

                vtx[i].uv = colorToUV(vertices[i].color); // no perspective correction
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<i32>& face : faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0]];

                for (size_t i = 1; i + 1 < face.size(); i++)
                {
                    const auto& v1 = vtx[face[i]];
                    const auto& v2 = vtx[face[i + 1]];

                    c.fillTriangleUV(v0.screen, v1.screen, v2.screen,
                        v0.uv, v1.uv, v2.uv,
                        *tex);
                }
            }
        }

        void drawObject(Canvas& c) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
                Color color;
            };

            std::vector<Vtx> vtx(vertices.size());

            // -----------------------------
            // STEP 1: vertex transform
            // -----------------------------
            for (size_t i = 0; i < vertices.size(); i++)
            {
                omni::Vec3<f32> v = vertices[i].position;

                v = rotate(v, rotation);
                v = translate(v, position);

                omni::Vec2<f32> proj = project(v);
                vtx[i].screen = normalizedToScreen(proj, c.getWidth(), c.getHeight());

                vtx[i].color = vertices[i].color; // no perspective correction
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<i32>& face : faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0]];

                for (size_t i = 1; i + 1 < face.size(); i++)
                {
                    const auto& v1 = vtx[face[i]];
                    const auto& v2 = vtx[face[i + 1]];

                    c.fillTriangle(v0.screen, v1.screen, v2.screen,
                        v0.color, v1.color, v2.color);
                }
            }
        }

        [[nodiscard]] inline omni::Vec3<f32> forward() const {
            return {
                sinf(rotation.y) * cosf(rotation.x),
              -sinf(rotation.x),
               cosf(rotation.y) * cosf(rotation.x)
            };
        }
    };

    struct Camera {
        omni::Vec3<f32> position = {};
        omni::Vec3<f32> rotation = {};

        void drawObjectWireframe(const Object3D& obj, const Canvas& c, const Color color, const f32 thickness) const {
            const omni::Vec3<f32> re_position = obj.position - position;
            for (const std::vector<i32>& face : obj.faces) {
                for (int i = 0; i < face.size(); i++) {
                    const omni::Vec3<f32> p0 = obj.vertices[face[i]].position;
                    const omni::Vec3<f32> p1 = obj.vertices[face[(i+1)%face.size()]].position;

                    const omni::Vec3<f32> p0CameraRelative = rotateInverse(translate(rotate(p0, obj.rotation), re_position), rotation);
                    const omni::Vec3<f32> p1CameraRelative = rotateInverse(translate(rotate(p1, obj.rotation), re_position), rotation);

                    if (p0CameraRelative.z < 0 || p1CameraRelative.z < 0) {
                        continue;
                    }

                    omni::Vec2<i32> s0 = normalizedToScreen(project(p0CameraRelative), c.getWidth(), c.getHeight());
                    omni::Vec2<i32> s1 = normalizedToScreen(project(p1CameraRelative), c.getWidth(), c.getHeight());
                    c.drawLine(s0, s1, color, thickness);
                }
            }
        }

        void drawObject_UV(const Object3D& obj, const Canvas& c, bool cullBackface = true) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
                omni::Vec2<f32> uv;
                float invZ;
                bool valid = true;
            };

            std::vector<Vtx> vtx(obj.vertices.size());

            const omni::Vec3<f32> re_position = obj.position - position;

            // -----------------------------
            // STEP 1: vertex transform
            // -----------------------------
            for (size_t i = 0; i < obj.vertices.size(); i++)
            {
                omni::Vec3<f32> v = obj.vertices[i].position;

                // object transform
                v = rotate(v, obj.rotation);
                v = translate(v, re_position);

                // camera transform
                v = rotateInverse(v, rotation);

                if (v.z <= 0.001f)
                {
                    vtx[i].valid = false;
                    continue;
                }

                float invZ = 1.0f / v.z;

                omni::Vec2<f32> proj = project(v);
                vtx[i].screen = normalizedToScreen(proj, c.getWidth(), c.getHeight());

                vtx[i].uv = colorToUV(obj.vertices[i].color);
                vtx[i].invZ = invZ;
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<i32>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0]];
                const auto& v1 = vtx[face[1]];
                const auto& v2 = vtx[face[2]];

                // skip degenerate / clipped triangles
                if (!v0.valid || !v1.valid || !v2.valid)
                    continue;

                // -----------------------------
                // BACKFACE CULLING (once per face)
                // -----------------------------
                if (cullBackface) {
                    const omni::Vec2<i32> e1 = v1.screen - v0.screen;
                    const omni::Vec2<i32> e2 = v2.screen - v0.screen;

                    const i32 cp = cross(e1, e2);

                    // IMPORTANT: adjust sign if needed
                    if (cp < 0)
                        continue;
                }

                // -----------------------------
                // TRIANGLE FAN RASTERIZATION
                // -----------------------------
                for (size_t i = 1; i + 1 < face.size(); i++)
                {
                    const auto& a = vtx[face[i]];
                    const auto& b = vtx[face[i + 1]];

                    if (!a.valid || !b.valid)
                        continue;

                    c.fillTriangleUV3D(
                        v0.screen, a.screen, b.screen,
                        v0.uv, a.uv, b.uv,
                        v0.invZ, a.invZ, b.invZ,
                        *obj.tex
                    );
                }
            }
        }

        void drawObject(const Object3D& obj, Canvas& c, bool cullBackface = true) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
                Color color;
                float invZ;
                bool valid = true;
            };

            std::vector<Vtx> vtx(obj.vertices.size());

            const omni::Vec3<f32> re_position = obj.position - position;

            // -----------------------------
            // STEP 1: vertex transform
            // -----------------------------
            for (size_t i = 0; i < obj.vertices.size(); i++)
            {
                omni::Vec3<f32> v = obj.vertices[i].position;

                v = rotate(v, obj.rotation);
                v = translate(v, re_position);
                v = rotateInverse(v, rotation);

                if (v.z <= 0.001f)
                {
                    vtx[i].valid = false;
                    continue;
                }

                float invZ = 1.0f / v.z;

                omni::Vec2<f32> proj = project(v);
                vtx[i].screen = normalizedToScreen(proj, c.getWidth(), c.getHeight());

                //vtx[i].color = obj.vertices[i].color;
                u8 z = static_cast<unsigned char>(std::clamp(invZ, 0.f, 1.f)*255);
                vtx[i].color = {z, z, z, 0xff};
                vtx[i].invZ = invZ;
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<i32>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0]];
                const auto& v1 = vtx[face[1]];
                const auto& v2 = vtx[face[2]];

                // skip invalid triangles
                if (!v0.valid || !v1.valid || !v2.valid)
                    continue;

                // -----------------------------
                // BACKFACE CULLING (ONCE)
                // -----------------------------
                if (cullBackface) {
                    const omni::Vec2<i32> e1 = v1.screen - v0.screen;
                    const omni::Vec2<i32> e2 = v2.screen - v0.screen;

                    const i32 cp = cross(e1, e2);

                    // IMPORTANT: adjust sign if needed
                    if (cp < 0)
                        continue;
                }

                // -----------------------------
                // TRIANGLE FAN RASTERIZATION
                // -----------------------------
                for (size_t i = 1; i + 1 < face.size(); i++)
                {
                    const auto& a = vtx[face[i]];
                    const auto& b = vtx[face[i + 1]];

                    if (!a.valid || !b.valid)
                        continue;

                    c.fillTriangle(
                        v0.screen, a.screen, b.screen,
                        v0.color, a.color, b.color
                    );
                }
            }
        }

        void drawObject(const Object3D& obj, Canvas& c, Canvas& zBuffer, bool cullBackface = true) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
                Color color;
                float invZ;
                bool valid = true;
            };

            std::vector<Vtx> vtx(obj.vertices.size());

            const omni::Vec3<f32> re_position = obj.position - position;

            // -----------------------------
            // STEP 1: vertex transform
            // -----------------------------
            for (size_t i = 0; i < obj.vertices.size(); i++)
            {
                omni::Vec3<f32> v = obj.vertices[i].position;

                v = rotate(v, obj.rotation);
                v = translate(v, re_position);
                v = rotateInverse(v, rotation);

                if (v.z <= 0.001f)
                {
                    vtx[i].valid = false;
                    continue;
                }

                float invZ = 1.0f / v.z;

                omni::Vec2<f32> proj = project(v);
                vtx[i].screen = normalizedToScreen(proj, c.getWidth(), c.getHeight());

                vtx[i].color = obj.vertices[i].color;
                //u8 z = static_cast<unsigned char>(std::clamp(invZ, 0.f, 1.f)*255);
                //vtx[i].color = {z, z, z, 0xff};
                vtx[i].invZ = invZ;
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<i32>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0]];
                const auto& v1 = vtx[face[1]];
                const auto& v2 = vtx[face[2]];

                // skip invalid triangles
                if (!v0.valid || !v1.valid || !v2.valid)
                    continue;

                // -----------------------------
                // BACKFACE CULLING (ONCE)
                // -----------------------------
                if (cullBackface) {
                    const omni::Vec2<i32> e1 = v1.screen - v0.screen;
                    const omni::Vec2<i32> e2 = v2.screen - v0.screen;

                    const i32 cp = cross(e1, e2);

                    // IMPORTANT: adjust sign if needed
                    if (cp < 0)
                        continue;
                }

                // -----------------------------
                // TRIANGLE FAN RASTERIZATION
                // -----------------------------
                for (size_t i = 1; i + 1 < face.size(); i++)
                {
                    const auto& a = vtx[face[i]];
                    const auto& b = vtx[face[i + 1]];

                    if (!a.valid || !b.valid)
                        continue;

                    c.fillTriangleZ(
                        v0.screen, a.screen, b.screen,
                        v0.color, a.color, b.color,
                        v0.invZ, v1.invZ, v2.invZ,
                        zBuffer
                    );
                }
            }
        }

        [[nodiscard]] inline omni::Vec3<f32> directionObj(const omni::Vec3<f32>& direction) const {
            return rotate(direction, {0, rotation.y, 0});
        }

        [[nodiscard]] inline omni::Vec3<f32> directionCam(const omni::Vec3<f32>& direction) const {
            return rotate(direction, rotation);
        }

        [[nodiscard]] inline omni::Vec3<f32> forwardCam() const {
            return rotate(Dir3D::FORWARD, rotation);
        }
        [[nodiscard]] inline omni::Vec3<f32> backwardCam() const { return forwardCam() * -1; }

        [[nodiscard]] inline omni::Vec3<f32> rightCam() const {
            return rotate(Dir3D::RIGHT, rotation);
        }
        [[nodiscard]] inline omni::Vec3<f32> leftCam() const { return rightCam() * -1; }

        [[nodiscard]] inline omni::Vec3<f32> upCam() const {
            return rotate(Dir3D::UP, rotation);
        }
        [[nodiscard]] inline omni::Vec3<f32> downCam() const { return upCam() * -1; }
    };

    inline Object3D loadOBJ(const std::string& fileName) {
        Object3D obj;

        std::ifstream file(fileName);
        if (!file.is_open()) {
            LOG_ERROR("Could not open file {}", fileName);
            return {};
        }

        std::string line;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            // ---- Vertices ----
            if (prefix == "v") {
                omni::Vec3<f32> v;
                ss >> v.x >> v.y >> v.z;
                obj.vertices.push_back({ v, randomUint32() });
            }

            // ---- Faces ----
            else if (prefix == "f") {
                std::vector<i32> face;
                std::string vert;

                while (ss >> vert) {
                    // handle formats like "1", "1/2", "1/2/3"
                    size_t slash = vert.find('/');
                    if (slash != std::string::npos)
                        vert = vert.substr(0, slash);

                    face.push_back(std::stoi(vert) - 1); // OBJ is 1-based
                }

                if (face.size() >= 3)
                    obj.faces.push_back(face);
            }
        }

        return obj;
    }

}

#endif //GRAPHITE_H
