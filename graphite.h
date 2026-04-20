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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

inline uint32_t randomUint32()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);

    return dist(gen);
}

/*
namespace GraphiteOld {
    using namespace omni::basic;

    struct Color {
        union {
            uint32_t color{};
            f32 fColor;
            struct { uint8_t r, g, b, a; };
            struct { u16 u, v; };
        };

        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF)
            : r(r), g(g), b(b), a(a) {}

        constexpr Color(u16 u, u16 v)
            : u(u), v(v) {}

        constexpr Color(uint32_t c = 0xFF000000)
            : color(c) {}

        constexpr Color(f32 c)
            : fColor(c) {}

        operator uint32_t() const { return color; }
        explicit operator f32() const { return fColor; }
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
        return {
            color.u / (f32)UINT16_MAX,
            color.v / (f32)UINT16_MAX
        };
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
                f32* zRow = reinterpret_cast<f32 *>(zBuffer.pixels) + y * WIDTH;

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

                        if (z < zRow[x]) {
                            w0 += dy12;
                            w1 += dy20;
                            w2 += dy01;
                            continue;
                        }

#ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], c);
#else
                        row[x] = c;
#endif
                        zRow[x] = z;
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
            f32 u = 1.0f - std::clamp(uv.u, 0.001f, 0.999f);
            f32 v = 1.0f - std::clamp(uv.v, 0.001f, 0.999f);

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

        void fillTriangleUVZ(
            i32 x1, i32 y1,
            i32 x2, i32 y2,
            i32 x3, i32 y3,
            const omni::Vec2<f32>& uvOverZ1,
            const omni::Vec2<f32>& uvOverZ2,
            const omni::Vec2<f32>& uvOverZ3,
            const f32 invZ1,
            const f32 invZ2,
            const f32 invZ3,
            const Canvas& tex,
            const Canvas& zBuffer) const
        {
            auto edge = [](i32 x0, i32 y0, i32 x1, i32 y1, i32 x, i32 y)
            {
                return (x - x0) * (y1 - y0) - (y - y0) * (x1 - x0);
            };

            const int area = edge(x1, y1, x2, y2, x3, y3);
            if (area == 0) return;

            const float invArea = 1.0f / (float)area;

            const int minX = std::max(0, std::min({x1, x2, x3}));
            const int maxX = std::min((int)WIDTH - 1, std::max({x1, x2, x3}));
            const int minY = std::max(0, std::min({y1, y2, y3}));
            const int maxY = std::min((int)HEIGHT - 1, std::max({y1, y2, y3}));

            if (minX > maxX || minY > maxY) return;

            const int dx01 = x2 - x1;
            const int dy01 = y2 - y1;

            const int dx12 = x3 - x2;
            const int dy12 = y3 - y2;

            const int dx20 = x1 - x3;
            const int dy20 = y1 - y3;

            int w0_row = edge(x2, y2, x3, y3, minX, minY);
            int w1_row = edge(x3, y3, x1, y1, minX, minY);
            int w2_row = edge(x1, y1, x2, y2, minX, minY);

            for (int y = minY; y <= maxY; ++y)
            {
                int w0 = w0_row;
                int w1 = w1_row;
                int w2 = w2_row;

                u32* row = pixels + y * WIDTH;
                f32* zRow = reinterpret_cast<f32*>(zBuffer.pixels) + y * WIDTH;

                for (int x = minX; x <= maxX; ++x)
                {
                    if ((w0 >= 0 && w1 >= 0 && w2 >= 0) ||
                        (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    {
                        const float b0 = w0 * invArea;
                        const float b1 = w1 * invArea;
                        const float b2 = w2 * invArea;

                        const f32 invZ =
                            invZ1 * b0 +
                            invZ2 * b1 +
                            invZ3 * b2;

                        if (invZ < zRow[x]) {
                            w0 += dy12;
                            w1 += dy20;
                            w2 += dy01;
                            continue;
                        }

                        const omni::Vec2<f32> uvOverZ =
                            uvOverZ1 * b0 +
                            uvOverZ2 * b1 +
                            uvOverZ3 * b2;

                        const omni::Vec2<f32> uv = uvOverZ / invZ;

                        const Color c = sampleUV(uv, tex);

        #ifdef ALPHA_BLEND
                        row[x] = mixColors(row[x], c);
        #else
                        row[x] = c;
        #endif

                        zRow[x] = invZ;
                    }

                    w0 += dy12;
                    w1 += dy20;
                    w2 += dy01;
                }

                w0_row -= dx12;
                w1_row -= dx20;
                w2_row -= dx01;
            }
        }

        void fillTriangleUVZ(
            const omni::Vec2<i32> p1,
            const omni::Vec2<i32> p2,
            const omni::Vec2<i32> p3,
            const omni::Vec2<f32>& uvOverZ1,
            const omni::Vec2<f32>& uvOverZ2,
            const omni::Vec2<f32>& uvOverZ3,
            const f32 invZ1,
            const f32 invZ2,
            const f32 invZ3,
            const Canvas& tex,
            const Canvas& zBuffer) const {
            fillTriangleUVZ(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, uvOverZ1, uvOverZ2, uvOverZ3, invZ1, invZ2, invZ3, tex, zBuffer);
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

    struct FaceIndex {
        i32 vertexIdx;
        i32 texCoordIdx;
        i32 normalIdx;
        Color randColor;
    };

    struct Object3D {
        std::vector<omni::Vec3<f32>> vertices;
        std::vector<omni::Vec3<f32>> normals;
        std::vector<omni::Vec2<f32>> texCoords;
        std::vector<std::vector<FaceIndex>> faces;
        omni::Vec3<f32> position = {};
        omni::Vec3<f32> rotation = {};
        Canvas* tex = nullptr;
    };

    struct Camera {
        omni::Vec3<f32> position = {};
        omni::Vec3<f32> rotation = {};

        void drawObjectWireframe(const Object3D& obj, const Canvas& c, const Color color, const f32 thickness) const {
            const omni::Vec3<f32> re_position = obj.position - position;
            for (const std::vector<FaceIndex>& face : obj.faces) {
                for (int i = 0; i < face.size(); i++) {
                    const omni::Vec3<f32> p0 = obj.vertices[face[i].vertexIdx];
                    const omni::Vec3<f32> p1 = obj.vertices[face[(i+1)%face.size()].vertexIdx];

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

        void drawObjectTexture(const Object3D& obj, Canvas& c, Canvas* zBuffer = nullptr, bool cullBackface = true) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
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
                omni::Vec3<f32> v = obj.vertices[i];

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

                vtx[i].invZ = invZ;
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<FaceIndex>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0].vertexIdx];
                const omni::Vec2<f32> uv0 = obj.texCoords[face[0].texCoordIdx];
                const auto& v1 = vtx[face[1].vertexIdx];
                const omni::Vec2<f32> uv1 = obj.texCoords[face[1].texCoordIdx];
                const auto& v2 = vtx[face[2].vertexIdx];
                const omni::Vec2<f32> uv2 = obj.texCoords[face[2].texCoordIdx];

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
                    const auto& a = vtx[face[i].vertexIdx];
                    const omni::Vec2<f32> auv = obj.texCoords[face[i].texCoordIdx];
                    const auto& b = vtx[face[i + 1].vertexIdx];
                    const omni::Vec2<f32> buv = obj.texCoords[face[i+1].texCoordIdx];

                    if (!a.valid || !b.valid)
                        continue;

                    c.fillTriangleUV3D(
                        v0.screen, a.screen, b.screen,
                        uv0*v0.invZ, auv*a.invZ, buv*b.invZ,
                        v0.invZ, a.invZ, b.invZ,
                        *obj.tex
                    );
                }
            }
        }

        void drawObjectColor(const Object3D& obj, Canvas& c, Canvas* zBuffer = nullptr, bool cullBackface = true) const {
            struct Vtx
            {
                omni::Vec2<i32> screen;
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
                omni::Vec3<f32> v = obj.vertices[i];

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

                vtx[i].invZ = invZ;
            }

            // -----------------------------
            // STEP 2: rasterize faces
            // -----------------------------
            for (const std::vector<FaceIndex>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0].vertexIdx];
                const Color c0 = face[0].randColor;
                const auto& v1 = vtx[face[1].vertexIdx];
                const Color c1 = face[1].randColor;
                const auto& v2 = vtx[face[2].vertexIdx];
                const Color c2 = face[2].randColor;

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
                    const auto& a = vtx[face[i].vertexIdx];
                    const Color ca = face[i].randColor;
                    const auto& b = vtx[face[i + 1].vertexIdx];
                    const Color cb = face[i + 1].randColor;

                    if (!a.valid || !b.valid)
                        continue;

                    if (zBuffer) {
                        c.fillTriangleZ(
                            v0.screen, a.screen, b.screen,
                            c0, ca, cb,
                            v0.invZ, a.invZ, b.invZ,
                            *zBuffer
                        );
                    } else {
                        c.fillTriangle(
                            v0.screen, a.screen, b.screen,
                            c0, ca, cb
                        );
                    }
                }
            }
        }

        void drawObject(const Object3D& obj, Canvas& c, Canvas* zBuffer = nullptr, bool cullBackface = true) const {
            if (obj.tex == nullptr) {
                drawObjectColor(obj, c, zBuffer, cullBackface);
            } else {
                drawObjectTexture(obj, c, zBuffer, cullBackface);
            }
        }

        void drawObjectDepth(const Object3D& obj, Canvas& c, Canvas* zBuffer = nullptr, bool cullBackface = true) const {
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
                omni::Vec3<f32> v = obj.vertices[i];

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
            for (const std::vector<FaceIndex>& face : obj.faces)
            {
                if (face.size() < 3) continue;

                const auto& v0 = vtx[face[0].vertexIdx];
                const auto& v1 = vtx[face[1].vertexIdx];
                const auto& v2 = vtx[face[2].vertexIdx];

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
                    const auto& a = vtx[face[i].vertexIdx];
                    const auto& b = vtx[face[i + 1].vertexIdx];

                    if (!a.valid || !b.valid)
                        continue;

                    if (zBuffer) {
                        c.fillTriangleZ(
                            v0.screen, a.screen, b.screen,
                            v0.color, a.color, b.color,
                            v0.invZ, a.invZ, b.invZ,
                            *zBuffer
                        );
                    } else {
                        c.fillTriangle(
                            v0.screen, a.screen, b.screen,
                            v0.color, a.color, b.color
                        );
                    }
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

        //std::vector<omni::Vec2<f32>> texCoords;
        std::string line;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            // ---- Vertex positions ----
            if (prefix == "v") {
                omni::Vec3<f32> v;
                ss >> v.x >> v.y >> v.z;

                obj.vertices.push_back(v);
            }

            // ---- Vertex normals ----
            if (prefix == "vn") {
                omni::Vec3<f32> n;
                ss >> n.x >> n.y >> n.z;

                obj.normals.push_back(n);
            }

            // ---- Texture coordinates ----
            else if (prefix == "vt") {
                omni::Vec2<f32> uv;
                ss >> uv.u >> uv.v;
                obj.texCoords.push_back(uv);
            }

            // ---- Faces ----
            else if (prefix == "f") {
                std::vector<FaceIndex> face;
                std::string vert;

                while (ss >> vert) {
                    // formats:
                    // v
                    // v/vt
                    // v/vt/vn
                    // v//vn

                    int vIndex  = -1;
                    int vnIndex = -1;
                    int vtIndex = -1;

                    size_t s1 = vert.find('/');
                    size_t s2 = std::string::npos;

                    if (s1 == std::string::npos) {
                        vIndex = std::stoi(vert) - 1;
                        //LOG_DEBUG("vIndex: {}", vIndex);
                    } else {
                        vIndex = std::stoi(vert.substr(0, s1)) - 1;

                        s2 = vert.find('/', s1 + 1);

                        std::string vtStr =
                            (s2 == std::string::npos)
                            ? vert.substr(s1 + 1)
                            : vert.substr(s1 + 1, s2 - s1 - 1);

                        if (!vtStr.empty())
                            vtIndex = std::stoi(vtStr) - 1;

                        //LOG_DEBUG("vIndex: {}, vtIndex: {}", vIndex, vtIndex);
                    }

                    face.push_back({
                        vIndex, vtIndex, 0, randomUint32()
                    });
                }

                if (face.size() >= 3)
                    obj.faces.push_back(face);
            }
        }

        return obj;
    }

}
*/

namespace Graphite {
    using namespace glm;
    struct NormalizedRectangle {
        int32 x1, y1, x2, y2;
        int32 ox1, oy1, ox2, oy2;

        bool normalizeRectangle(const int32 x, const int32 y, const int32 w, const int32 h, const int32 canvas_width, const int32 canvas_height) {
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
            x1 = glm::max(0, glm::min(ox1, canvas_width - 1));
            y1 = glm::max(0, glm::min(oy1, canvas_height - 1));
            x2 = glm::max(0, glm::min(ox2, canvas_width - 1));
            y2 = glm::max(0, glm::min(oy2, canvas_height - 1));

            // ---- STEP 4: safety check (important) ----
            if (x1 > x2 || y1 > y2)
                return false;

            return true;
        }
    };

    // ==========================================
    //                  COLORS
    // ==========================================

    struct Color {
        union {
            uint32 color{};
            float32 fColor;
            struct { uint8 r, g, b, a; };
            struct { uint16 u, v; };
        };

        constexpr Color(const uint8 r, const uint8 g, const uint8 b, const uint8 a = 0xFF)
            : r(r), g(g), b(b), a(a) {}

        constexpr Color(const uint16 u, const uint16 v)
            : u(u), v(v) {}

        explicit constexpr Color(const uint32 c = 0xFF000000)
            : color(c) {}

        explicit constexpr Color(const float32 c)
            : fColor(c) {}

        explicit operator uint32() const { return color; }
        explicit operator float32() const { return fColor; }
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
    constexpr Color HSLtoRGB (float32 hueAngleDeg, float32 satNorm, float32 lightNorm, uint8 alpha);
    // h = 0..360, s = 0..1, v = 0..1
    constexpr Color HSVtoRGB (float32 hueAngleDeg, float32 satNorm, float32 valueNorm, uint8 alpha);

    uint8_t mixComponent (uint16 c1, uint16 c2, uint16 a);
    Color mixColors (Color c1, Color c2);
    Color mixColorsTri (Color c1, Color c2, Color c3, float32 w0, float32 w1, float32 w2);
    float mixDepths(float32 c1, float32 c2, float32 c3, float32 w0, float32 w1, float32 w2);

    // ==========================================
    //                  CANVAS
    // ==========================================

    class Canvas {
    public:
        // ====================== CONSTRUCTORS ======================
        Canvas(uint32 width, uint32 height, u32 stride);
        Canvas(uint32 width, uint32 height);
        explicit Canvas(const std::string& spriteFilepath);
        Canvas();
        ~Canvas();

        // ====================== INITIALIZERS ======================
        void linkCanvas(uint32* pixelLink, uint32 width, uint32 height);
        void newCanvas(uint32 width, uint32 height);
        void newCanvas(uint32 width, uint32 height, uint32 stride);

        // ====================== ACCESSORS =========================
        [[nodiscard]] uint32 getWidth () const;
        [[nodiscard]] uint32 getHeight() const;
        [[nodiscard]] uint32 getStride () const;
        void setWidth  (uint32 width);
        void setHeight (uint32 height);
        void setStride (uint32 stride);

        // ===================== SUB-CANVASING ======================
        [[nodiscard]] Canvas subcanvas(int32 x, int32 y, int32 w, int32 h) const;
        [[nodiscard]] Canvas subcanvas(glm::i32vec2 p, int32 w, int32 h) const;

        // ==================== FULL-CANVAS FILL ====================
        void fill       (Color color) const;
        void blitCanvas (const Canvas& sourceCanvas) const;

        // ======================== DRAWING =========================
        void fillRect (int32 x0, int32 y0, int32 width, int32 height, Color color) const;
        void fillRect (glm::i32vec2 p0, glm::i32vec2 dim, Color color) const;

        void fillCircle  (i32 cx, i32 cy, i32 radius, Color color) const;
        void fillCircle  (const glm::i32vec2& p, i32 radius, Color color) const;
        void fillEllipse (i32 cx, i32 cy, i32 rx, i32 ry, Color color) const;
        void fillEllipse (const glm::i32vec2& p, const glm::i32vec2& radii, Color color) const;

        // 1. flat color, no z
        void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, Color color) const;
        void fillTriangle(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3, Color color) const;

        // 2. per-vertex color, optional z-buffer
        void fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
                          Color c1, Color c2, Color c3,
                          f32 invZ1 = 0, f32 invZ2 = 0, f32 invZ3 = 0,
                          const Canvas* zBuffer = nullptr) const;
        void fillTriangle(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3,
                          Color c1, Color c2, Color c3,
                          f32 invZ1 = 0, f32 invZ2 = 0, f32 invZ3 = 0,
                          const Canvas* zBuffer = nullptr) const;

        // 3. textured, optional z-buffer (perspective correct)
        void fillTriangleUV(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
                            const glm::vec2& uvOverZ1, const glm::vec2& uvOverZ2, const glm::vec2& uvOverZ3,
                            f32 invZ1, f32 invZ2, f32 invZ3,
                            const Canvas& tex, const Canvas* zBuffer = nullptr) const;
        void fillTriangleUV(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3,
                            const glm::vec2& uvOverZ1, const glm::vec2& uvOverZ2, const glm::vec2& uvOverZ3,
                            f32 invZ1, f32 invZ2, f32 invZ3,
                            const Canvas& tex, const Canvas* zBuffer = nullptr) const;

        static float distToSegment (const glm::vec2& p, const glm::vec2& a, const glm::vec2& b);

        void drawPoint(i32 x, i32 y, i32 radius, Color color) const;
        void drawPoint(const i32vec2& p, i32 radius, Color color) const;

        void drawLine (i32 x1, i32 y1, i32 x2, i32 y2, Color color) const;
        void drawLine (const glm::ivec2& p1, const glm::ivec2& p2, Color color) const;
        void drawLine (i32 x1, i32 y1, i32 x2, i32 y2, Color color, float thickness) const;
        void drawLine (const glm::ivec2& p1, const glm::ivec2& p2, Color color, float thickness) const;

        void drawCanvas(i32 x0, i32 y0, i32 width, i32 height, const Canvas& sourceCanvas) const;
        void drawCanvas(const glm::ivec2& p, i32 width, i32 height, const Canvas& sourceCanvas) const;

        void writeCharBaseline   (char c, int32 x, int32 y, int32 font_size, Color color) const;
        void writeCharBaseline   (char c, const glm::i32vec2& p, int32 font_size, Color color) const;
        void writeStringBaseline (const std::string& s, int32 x, int32 y, int32 font_size, Color color) const;
        void writeStringBaseline (const std::string& s, const glm::i32vec2& p, int32 font_size, Color color) const;

        // ====================== FILE OUTPUT =======================
        [[nodiscard]] bool saveToPPM (const std::string& filename) const;
        [[nodiscard]] bool saveToJPG (const std::string& filename, int quality = 100) const;
        [[nodiscard]] bool saveToPNG (const std::string& filename) const;
        [[nodiscard]] bool saveToBMP (const std::string& filename) const;

        [[nodiscard]] Color getPixelColor(uint32 x, uint32 y) const;

    protected:
        uint32 *pixels;
        uint32 WIDTH;
        uint32 HEIGHT;
        uint32 STRIDE;

        bool linked = false;
        unsigned char* rawData = nullptr;

        [[nodiscard]] uint32* getPixel(uint32 x, uint32 y) const;
        [[nodiscard]] uint32 getPixelIndex(uint32 x, uint32 y) const;
    };

    // ==========================================
    //                3-DIMENSIONS
    // ==========================================

    glm::ivec2 normalizedToScreen(const glm::vec2& p, u32 WIDTH, u32 HEIGHT);
    glm::vec2 project(const glm::vec3& p);
    glm::vec3 translate(const glm::vec3& p, const glm::vec3& translation);

    // rotation: {pitch, yaw, roll} aka {around x, around y, around z}
    glm::vec3 rotate(const glm::vec3& p, const glm::vec3& rotation);
    glm::vec3 rotateInverse(const glm::vec3& p, const glm::vec3& rotation);
    glm::mat3 makeRotationMatrix(const glm::vec3& rotation);

    namespace Dir3D {
        inline const f32vec3 UP       {0,  1,  0};
        inline const f32vec3 DOWN     {0, -1,  0};
        inline const f32vec3 RIGHT    {1,  0,  0};
        inline const f32vec3 LEFT     {-1, 0,  0};
        inline const f32vec3 FORWARD  {0,  0,  1};
        inline const f32vec3 BACKWARD {0,  0,  -1};
    }

    struct FaceIndex {
        i32 vertexIdx{};
        i32 texCoordIdx{};
        i32 normalIdx{};
        Color randColor;
    };

    struct Object3D {
        std::vector<glm::f32vec3> vertices;
        std::vector<glm::f32vec3> normals;
        std::vector<glm::f32vec2> texCoords;
        std::vector<std::vector<FaceIndex>> faces;
        glm::f32vec3 position{};
        glm::f32vec3 rotation{};
        glm::f32vec3 scale{};
        Canvas* tex = nullptr;
    };

    Object3D loadOBJ(const std::string& fileName);

    struct Camera {
        glm::f32vec3 position;
        glm::f32vec3 rotation;

        void drawObjectWireframe(const Object3D& obj, const Canvas& c, Color color, f32 thickness) const;

        [[nodiscard]] glm::vec3 directionObj(const glm::vec3& direction) const;
        [[nodiscard]] glm::vec3 directionCam(const glm::vec3& direction) const;

        [[nodiscard]] glm::vec3 forwardCam()  const;
        [[nodiscard]] glm::vec3 backwardCam() const;
        [[nodiscard]] glm::vec3 rightCam()  const;
        [[nodiscard]] glm::vec3 leftCam()   const;
        [[nodiscard]] glm::vec3 upCam()   const;
        [[nodiscard]] glm::vec3 downCam() const;

        [[nodiscard]] glm::vec3 forwardObj()  const;
        [[nodiscard]] glm::vec3 backwardObj() const;
        [[nodiscard]] glm::vec3 rightObj()  const;
        [[nodiscard]] glm::vec3 leftObj()   const;
        [[nodiscard]] glm::vec3 upObj()   const;
        [[nodiscard]] glm::vec3 downObj() const;

        void drawObject       (const Object3D& obj, const Canvas& c, const Canvas* zBuffer = nullptr, bool cullBackface = true) const;

        void drawObjectTexture(const Object3D& obj, const Canvas& c, const Canvas* zBuffer = nullptr, bool cullBackface = true) const;
        void drawObjectColor  (const Object3D& obj, const Canvas& c, const Canvas* zBuffer = nullptr, bool cullBackface = true) const;
        void drawObjectDepth  (const Object3D& obj, const Canvas& c, const Canvas* zBuffer = nullptr, bool cullBackface = true) const;

    private:
        struct Vtx {
            glm::ivec2 screen{};
            float invZ{};
            bool valid = false;
        };

        [[nodiscard]] std::vector<Vtx> transformVertices(const Object3D& obj, const Canvas& c) const;
        static bool backfaceCull(const Vtx& v0, const Vtx& v1, const Vtx& v2);
    };























    // ==========================================
    //              IMPLEMENTATIONS
    // ==========================================

#ifndef GRAPHITE_NO_IMPLEMENTATION

    constexpr Color HSLtoRGB (float32 hueAngleDeg, float32 satNorm, float32 lightNorm, const uint8 alpha = 255) {
        hueAngleDeg = glm::mod(hueAngleDeg, 360.0f);
        if (hueAngleDeg < 0.0f) hueAngleDeg += 360.0f;

        satNorm = glm::clamp(satNorm, 0.0f, 1.0f);
        lightNorm = glm::clamp(lightNorm, 0.0f, 1.0f);

        const float32 c = (1.0f - glm::abs(2.0f * lightNorm - 1.0f)) * satNorm;
        const float32 x = c * (1.0f - glm::abs(glm::mod(hueAngleDeg / 60.0f, 2.0f) - 1.0f));
        const float32 m = lightNorm - c * 0.5f;

        float32 r1 = 0, g1 = 0, b1 = 0;

        if      (hueAngleDeg < 60)  { r1 = c; g1 = x; b1 = 0; }
        else if (hueAngleDeg < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (hueAngleDeg < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (hueAngleDeg < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (hueAngleDeg < 300) { r1 = x; g1 = 0; b1 = c; }
        else              { r1 = c; g1 = 0; b1 = x; }

        const auto r = static_cast<uint8>(glm::round((r1 + m) * 255.0f));
        const auto g = static_cast<uint8>(glm::round((g1 + m) * 255.0f));
        const auto b = static_cast<uint8>(glm::round((b1 + m) * 255.0f));

        return {r, g, b, alpha};
    }
    constexpr Color HSVtoRGB (float32 hueAngleDeg, float32 satNorm, float32 valueNorm, const uint8 alpha = 255) {
        hueAngleDeg = glm::mod(hueAngleDeg, 360.0f);
        if (hueAngleDeg < 0.0f) hueAngleDeg += 360.0f;

        satNorm = glm::clamp(satNorm, 0.0f, 1.0f);
        valueNorm = glm::clamp(valueNorm, 0.0f, 1.0f);

        const float32 c = valueNorm * satNorm;
        const float32 x = c * (1.0f - glm::abs(glm::mod(hueAngleDeg / 60.0f, 2.0f) - 1.0f));
        const float32 m = valueNorm - c;

        float32 r1 = 0, g1 = 0, b1 = 0;

        if      (hueAngleDeg < 60)  { r1 = c; g1 = x; b1 = 0; }
        else if (hueAngleDeg < 120) { r1 = x; g1 = c; b1 = 0; }
        else if (hueAngleDeg < 180) { r1 = 0; g1 = c; b1 = x; }
        else if (hueAngleDeg < 240) { r1 = 0; g1 = x; b1 = c; }
        else if (hueAngleDeg < 300) { r1 = x; g1 = 0; b1 = c; }
        else              { r1 = c; g1 = 0; b1 = x; }

        const auto r = static_cast<uint8>(std::lround((r1 + m) * 255.0f));
        const auto g = static_cast<uint8>(std::lround((g1 + m) * 255.0f));
        const auto b = static_cast<uint8>(std::lround((b1 + m) * 255.0f));

        return {r, g, b, alpha};
    }

    inline uint8_t mixComponent (const uint16 c1, const uint16 c2, const uint16 a) { return c1 + (c2 - c1)*a/255; }
    inline Color mixColors      (const Color c1, const Color c2) {
        return {
            mixComponent(c1.r, c2.r, c2.a),
            mixComponent(c1.g, c2.g, c2.a),
            mixComponent(c1.b, c2.b, c2.a),
            mixComponent(c1.a, c2.a, c2.a),
        };
    }
    inline Color mixColorsTri(const Color c1, const Color c2, const Color c3, const float32 w0, const float32 w1, const float32 w2) {
        const glm::vec4 v = glm::vec4(c1.r, c1.g, c1.b, c1.a) * w0
                          + glm::vec4(c2.r, c2.g, c2.b, c2.a) * w1
                          + glm::vec4(c3.r, c3.g, c3.b, c3.a) * w2;
        return { static_cast<uint8>(v.r), static_cast<uint8>(v.g),
                 static_cast<uint8>(v.b), static_cast<uint8>(v.a) };
    }
    inline float mixDepths      (const float32 c1, const float32 c2, const float32 c3, const float32 w0, const float32 w1, const float32 w2) {
        return c1 * w0 + c2 * w1 + c3 * w2;
    }

    inline Canvas::Canvas  (const uint32 width, const uint32 height, const uint32 stride) {
        pixels = new uint32[width * height];
        WIDTH = width;
        HEIGHT = height;
        STRIDE = stride;
    }
    inline Canvas::Canvas  (const uint32 width, const uint32 height) {
        pixels = new uint32[width * height];
        WIDTH = width;
        HEIGHT = height;
        STRIDE = width;
    }
    inline Canvas::Canvas  (const std::string& spriteFilepath) {
        int32 x;
        int32 y;
        int32 n;
        rawData = stbi_load(spriteFilepath.c_str(), &x, &y, &n, 4);
        if (!rawData) {
            omni::LOG_ERROR("failed to load sprite file {}", spriteFilepath);
            return;
        }
        pixels = reinterpret_cast<uint32*>(rawData);
        WIDTH = static_cast<uint32>(x);
        HEIGHT = static_cast<uint32>(y);
        STRIDE = static_cast<uint32>(x);
    }
    inline Canvas::Canvas  () {
        pixels = nullptr;
        WIDTH = 0;
        HEIGHT = 0;
        STRIDE = 0;
    }
    inline Canvas::~Canvas () {
        if (rawData != nullptr)
            stbi_image_free(rawData);
        else if (!linked)
            delete[] pixels;
    }

    inline void Canvas::linkCanvas (uint32* pixelLink, const uint32 width, const uint32 height) {
        linked = true;
        pixels = pixelLink;
        WIDTH = width;
        HEIGHT = height;
        STRIDE = width;
    }
    inline void Canvas::newCanvas  (const uint32 width, const uint32 height) {
        delete[] pixels;
        pixels = new uint32[width * height];
        WIDTH = width;
        HEIGHT = height;
        STRIDE = width;
    }
    inline void Canvas::newCanvas  (const uint32 width, const uint32 height, const uint32 stride) {
        delete[] pixels;
        pixels = new uint32[width * height];
        WIDTH = width;
        HEIGHT = height;
        STRIDE = stride;
    }

    inline uint32 Canvas::getWidth  () const { return WIDTH; }
    inline uint32 Canvas::getHeight () const { return HEIGHT; }
    inline uint32 Canvas::getStride () const { return STRIDE; }
    inline void   Canvas::setWidth  (const uint32 width) { WIDTH = width; }
    inline void   Canvas::setHeight (const uint32 height) { HEIGHT = height; }
    inline void   Canvas::setStride (const uint32 stride) { STRIDE = stride; }

    inline Canvas Canvas::subcanvas(const int32 x, const int32 y, const int32 w, const int32 h) const {
        NormalizedRectangle nr{};
        Canvas c(0, 0);
        if (!nr.normalizeRectangle(x, y, w, h, static_cast<int32>(WIDTH), static_cast<int32>(HEIGHT))) return c;
        c.pixels = getPixel(x, y);
        c.setWidth(nr.x2 - nr.x1 + 1);
        c.setHeight(nr.y2 - nr.y1 + 1);
        return c;
    }
    inline Canvas Canvas::subcanvas(const glm::i32vec2 p, const int32 w, const int32 h) const {
        return subcanvas(p.x, p.y, w, h);
    }

    inline void Canvas::fill(const Color color) const {
        if (const auto c = static_cast<uint32>(color); (c & 0xFF) == ((c >> 8) & 0xFF) &&
                                                         (c & 0xFF) == ((c >> 16) & 0xFF) &&
                                                         (c & 0xFF) == ((c >> 24) & 0xFF)) {
            memset(pixels, static_cast<int32>(c) & 0xFF, sizeof(uint32) * WIDTH * HEIGHT);
            } else {
                std::fill_n(pixels, WIDTH * HEIGHT, c);
            }
    }

    inline void Canvas::fillRect(const int32 x0, const int32 y0, const int32 width, const int32 height, const Color color) const {
        NormalizedRectangle nr{};
        if (!nr.normalizeRectangle(x0, y0, width, height, static_cast<int32>(WIDTH), static_cast<int32>(HEIGHT))) return;
        const int32 rowWidth = nr.x2 - nr.x1 + 1;
        for (int32 y = nr.y1; y <= nr.y2; ++y) {
            uint32* row = pixels + y * STRIDE + nr.x1;
#ifdef ALPHA_BLEND
            if (color.a == 0xFF) {
                std::fill_n(row, rowWidth, static_cast<uint32>(color));
            } else {
                for (int32 x = 0; x < rowWidth; ++x)
                    row[x] = static_cast<uint32>(mixColors(Color(row[x]), color));
            }
#else
            std::fill_n(row, rowWidth, static_cast<uint32>(color));
#endif
        }
    }
    inline void Canvas::fillRect(const glm::i32vec2 p0, const glm::i32vec2 dim, const Color color) const {
        fillRect(p0.x, p0.y, dim.x, dim.y, color);
    }

    inline void Canvas::fillCircle(const i32 cx, const i32 cy, const i32 radius, const Color color) const {
        const i32 r2 = radius * radius;
        const i32 y1 = glm::max(0, cy - radius);
        const i32 y2 = glm::min(static_cast<i32>(HEIGHT) - 1, cy + radius);

        for (i32 y = y1; y <= y2; ++y) {
            const i32 dy  = y - cy;
            const i32 dy2 = dy * dy;
            const i32 xSpan = static_cast<i32>(sqrtf(static_cast<f32>(r2 - dy2)));

            const i32 xStart = glm::max(0,                          cx - xSpan);
            const i32 xEnd   = glm::min(static_cast<i32>(WIDTH) - 1, cx + xSpan);

            uint32* row = pixels + y * STRIDE;

#ifdef ALPHA_BLEND
            if (color.a == 0xFF) {
                std::fill_n(row + xStart, xEnd - xStart + 1, static_cast<u32>(color));
            } else {
                for (i32 x = xStart; x <= xEnd; ++x)
                    row[x] = static_cast<u32>(mixColors(Color(row[x]), color));
            }
#else
            std::fill_n(row + xStart, xEnd - xStart + 1, static_cast<u32>(color));
#endif
        }
    }
    inline void Canvas::fillCircle(const glm::i32vec2& p, const i32 radius, const Color color) const {
        fillCircle(p.x, p.y, radius, color);
    }
    inline void Canvas::fillEllipse(const i32 cx, const i32 cy, const i32 rx, const i32 ry, const Color color) const {
        const i32 y1 = glm::max(0, cy - ry);
        const i32 y2 = glm::min(static_cast<i32>(HEIGHT) - 1, cy + ry);

        const f32 ry2 = static_cast<f32>(ry * ry);

        for (i32 y = y1; y <= y2; ++y) {
            const f32 dy = static_cast<f32>(y - cy);
            const i32 xSpan = static_cast<i32>(static_cast<f32>(rx) * sqrtf(glm::max(0.0f, 1.0f - (dy * dy) / ry2)));

            const i32 xStart = glm::max(0,                           cx - xSpan);
            const i32 xEnd   = glm::min(static_cast<i32>(WIDTH) - 1, cx + xSpan);

            uint32* row = pixels + y * STRIDE;

#ifdef ALPHA_BLEND
            if (color.a == 0xFF) {
                std::fill_n(row + xStart, xEnd - xStart + 1, static_cast<u32>(color));
            } else {
                for (i32 x = xStart; x <= xEnd; ++x)
                    row[x] = static_cast<u32>(mixColors(Color(row[x]), color));
            }
#else
            std::fill_n(row + xStart, xEnd - xStart + 1, static_cast<u32>(color));
#endif
        }
    }
    inline void Canvas::fillEllipse(const glm::i32vec2& p, const glm::i32vec2& radii, const Color color) const {
        fillEllipse(p.x, p.y, radii.x, radii.y, color);
    }

    inline Color sampleUV(const glm::vec2& uv, const Canvas& tex) {
        const f32 u = 1.0f - glm::clamp(uv.x, 0.001f, 0.999f);
        const f32 v = 1.0f - glm::clamp(uv.y, 0.001f, 0.999f);
        return tex.getPixelColor(
            static_cast<u32>(u * static_cast<f32>(tex.getWidth())),
            static_cast<u32>(v * static_cast<f32>(tex.getHeight()))
        );
    };

    inline void blendPixel(u32& px, const Color c) {
#ifdef ALPHA_BLEND
        if (c.a == 0xFF)
            px = static_cast<u32>(c);
        else
            px = static_cast<u32>(mixColors(Color(px), c));
#else
        px = static_cast<u32>(c);
#endif
    }

    inline void Canvas::fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3, const Color color) const {
        auto edge = [](i32 ax, i32 ay, i32 bx, i32 by, i32 x, i32 y) {
            return (x - ax) * (by - ay) - (y - ay) * (bx - ax);
        };
        const int area = edge(x1, y1, x2, y2, x3, y3);
        if (area == 0) return;

        const int minX = std::max(0,                            std::min({x1, x2, x3}));
        const int maxX = std::min(static_cast<int>(WIDTH) - 1,  std::max({x1, x2, x3}));
        const int minY = std::max(0,                            std::min({y1, y2, y3}));
        const int maxY = std::min(static_cast<int>(HEIGHT) - 1, std::max({y1, y2, y3}));
        if (minX > maxX || minY > maxY) return;

        const int dy12 = y3 - y2, dy20 = y1 - y3, dy01 = y2 - y1;
        const int dx12 = x3 - x2, dx20 = x1 - x3, dx01 = x2 - x1;

        int w0_row = edge(x2, y2, x3, y3, minX, minY);
        int w1_row = edge(x3, y3, x1, y1, minX, minY);
        int w2_row = edge(x1, y1, x2, y2, minX, minY);

        for (int y = minY; y <= maxY; ++y) {
            int w0 = w0_row, w1 = w1_row, w2 = w2_row;
            u32* row = pixels + y * STRIDE;
            for (int x = minX; x <= maxX; ++x) {
                if ((w0|w1|w2) >= 0 || ((-w0)|(-w1)|(-w2)) >= 0) // see note
                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0))
                    blendPixel(row[x], color);
                w0 += dy12; w1 += dy20; w2 += dy01;
            }
            w0_row -= dx12; w1_row -= dx20; w2_row -= dx01;
        }
    }
    inline void Canvas::fillTriangle(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3, const Color color) const {
        fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, color);
    }

    inline void Canvas::fillTriangle(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
                      const Color c1, const Color c2, const Color c3,
                      const f32 invZ1, const f32 invZ2, const f32 invZ3,
                      const Canvas* zBuffer) const {
        auto edge = [](i32 ax, i32 ay, i32 bx, i32 by, i32 x, i32 y) {
            return (x - ax) * (by - ay) - (y - ay) * (bx - ax);
        };
        const int area = edge(x1, y1, x2, y2, x3, y3);
        if (area == 0) return;
        const float invArea = 1.0f / area;

        const int minX = std::max(0,                            std::min({x1, x2, x3}));
        const int maxX = std::min(static_cast<int>(WIDTH) - 1,  std::max({x1, x2, x3}));
        const int minY = std::max(0,                            std::min({y1, y2, y3}));
        const int maxY = std::min(static_cast<int>(HEIGHT) - 1, std::max({y1, y2, y3}));
        if (minX > maxX || minY > maxY) return;

        const int dy12 = y3-y2, dy20 = y1-y3, dy01 = y2-y1;
        const int dx12 = x3-x2, dx20 = x1-x3, dx01 = x2-x1;

        int w0_row = edge(x2, y2, x3, y3, minX, minY);
        int w1_row = edge(x3, y3, x1, y1, minX, minY);
        int w2_row = edge(x1, y1, x2, y2, minX, minY);

        for (int y = minY; y <= maxY; ++y) {
            int w0 = w0_row, w1 = w1_row, w2 = w2_row;
            u32*  row  = pixels + y * STRIDE;
            f32*  zRow = zBuffer ? reinterpret_cast<f32*>(zBuffer->pixels) + y * STRIDE : nullptr;

            for (int x = minX; x <= maxX; ++x) {
                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    const float b0 = w0 * invArea;
                    const float b1 = w1 * invArea;
                    const float b2 = w2 * invArea;

                    if (zRow) {
                        const f32 invZ = invZ1*b0 + invZ2*b1 + invZ3*b2;
                        if (invZ < zRow[x]) { w0+=dy12; w1+=dy20; w2+=dy01; continue; }
                        zRow[x] = invZ;
                    }

                    blendPixel(row[x], mixColorsTri(c1, c2, c3, b0, b1, b2));
                }
                w0 += dy12; w1 += dy20; w2 += dy01;
            }
            w0_row -= dx12; w1_row -= dx20; w2_row -= dx01;
        }
    }
    inline void Canvas::fillTriangle(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3,
                      const Color c1, const Color c2, const Color c3,
                      const f32 invZ1, const f32 invZ2, const f32 invZ3,
                      const Canvas* zBuffer) const {
        fillTriangle(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, c1, c2, c3, invZ1, invZ2, invZ3, zBuffer);
    }

    inline void Canvas::fillTriangleUV(i32 x1, i32 y1, i32 x2, i32 y2, i32 x3, i32 y3,
                        const glm::vec2& uvOverZ1, const glm::vec2& uvOverZ2, const glm::vec2& uvOverZ3,
                        const f32 invZ1, const f32 invZ2, const f32 invZ3,
                        const Canvas& tex, const Canvas* zBuffer) const {
        auto edge = [](i32 ax, i32 ay, i32 bx, i32 by, i32 x, i32 y) {
            return (x - ax) * (by - ay) - (y - ay) * (bx - ax);
        };
        const int area = edge(x1, y1, x2, y2, x3, y3);
        if (area == 0) return;
        const float invArea = 1.0f / area;

        const int minX = std::max(0,                            std::min({x1, x2, x3}));
        const int maxX = std::min(static_cast<int>(WIDTH) - 1,  std::max({x1, x2, x3}));
        const int minY = std::max(0,                            std::min({y1, y2, y3}));
        const int maxY = std::min(static_cast<int>(HEIGHT) - 1, std::max({y1, y2, y3}));
        if (minX > maxX || minY > maxY) return;

        const int dy12 = y3-y2, dy20 = y1-y3, dy01 = y2-y1;
        const int dx12 = x3-x2, dx20 = x1-x3, dx01 = x2-x1;

        int w0_row = edge(x2, y2, x3, y3, minX, minY);
        int w1_row = edge(x3, y3, x1, y1, minX, minY);
        int w2_row = edge(x1, y1, x2, y2, minX, minY);

        for (int y = minY; y <= maxY; ++y) {
            int w0 = w0_row, w1 = w1_row, w2 = w2_row;
            u32* row  = pixels + y * STRIDE;
            f32* zRow = zBuffer ? reinterpret_cast<f32*>(zBuffer->pixels) + y * STRIDE : nullptr;

            for (int x = minX; x <= maxX; ++x) {
                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    const float b0 = w0 * invArea;
                    const float b1 = w1 * invArea;
                    const float b2 = w2 * invArea;

                    const f32 invZ = invZ1*b0 + invZ2*b1 + invZ3*b2;

                    if (zRow) {
                        if (invZ < zRow[x]) { w0+=dy12; w1+=dy20; w2+=dy01; continue; }
                        zRow[x] = invZ;
                    }

                    const glm::vec2 uv = (uvOverZ1*b0 + uvOverZ2*b1 + uvOverZ3*b2) / invZ;
                    blendPixel(row[x], sampleUV(uv, tex));
                }
                w0 += dy12; w1 += dy20; w2 += dy01;
            }
            w0_row -= dx12; w1_row -= dx20; w2_row -= dx01;
        }
    }
    inline void Canvas::fillTriangleUV(const glm::ivec2& p1, const glm::ivec2& p2, const glm::ivec2& p3,
                        const glm::vec2& uvOverZ1, const glm::vec2& uvOverZ2, const glm::vec2& uvOverZ3,
                        const f32 invZ1, const f32 invZ2, const f32 invZ3,
                        const Canvas& tex, const Canvas* zBuffer) const {
        fillTriangleUV(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y, uvOverZ1, uvOverZ2, uvOverZ3, invZ1, invZ2, invZ3, tex, zBuffer);
    }

    inline void Canvas::drawPoint(const i32 x, const i32 y, const i32 radius, const Color color) const {
        const i32 rx2 = radius * 2;
        fillRect(x - radius, y - radius, rx2, rx2, color);
    }
    inline void Canvas::drawPoint(const i32vec2& p, const i32 radius, const Color color) const {
        drawPoint(p.x, p.y, radius, color);
    }

    inline void Canvas::drawLine(i32 x1, i32 y1, const i32 x2, const i32 y2, const Color color) const {
        const i32 dx  =  glm::abs(x2 - x1);
        const i32 dy  =  glm::abs(y2 - y1);
        const i32 sx  = (x1 < x2) ? 1 : -1;
        const i32 sy  = (y1 < y2) ? 1 : -1;
        i32 err = dx - dy;

        while (true) {
            if (static_cast<u32>(x1) < WIDTH && static_cast<u32>(y1) < HEIGHT) {
                uint32& px = pixels[y1 * STRIDE + x1];
#ifdef ALPHA_BLEND
                if (color.a == 0xFF)
                    px = static_cast<u32>(color);
                else
                    px = static_cast<u32>(mixColors(Color(px), color));
#else
                px = static_cast<u32>(color);
#endif
            }

            if (x1 == x2 && y1 == y2) break;

            const i32 e2 = 2 * err;
            if (e2 > -dy) { err -= dy; x1 += sx; }
            if (e2 <  dx) { err += dx; y1 += sy; }
        }
    }
    inline void Canvas::drawLine(const glm::ivec2& p1, const glm::ivec2& p2, const Color color) const {
        drawLine(p1.x, p1.y, p2.x, p2.y, color);
    }

    inline float Canvas::distToSegment(const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) {
        const glm::vec2 v  = b - a;
        const glm::vec2 w  = p - a;
        const float     c1 = glm::dot(w, v);
        if (c1 <= 0) return glm::length(w);
        const float c2 = glm::dot(v, v);
        if (c2 <= c1) return glm::length(p - b);
        return glm::length(p - (a + (c1 / c2) * v));
    }
    inline void Canvas::drawLine(const i32 x1, const i32 y1, const i32 x2, const i32 y2, const Color color, const float thickness) const {
        const float r = thickness * 0.5f;

        const int minX = glm::max(0,                  (int)glm::min(x1, x2) - (int)thickness);
        const int maxX = glm::min((int)WIDTH  - 1,    (int)glm::max(x1, x2) + (int)thickness);
        const int minY = glm::max(0,                  (int)glm::min(y1, y2) - (int)thickness);
        const int maxY = glm::min((int)HEIGHT - 1,    (int)glm::max(y1, y2) + (int)thickness);

        const glm::vec2 a(x1, y1);
        const glm::vec2 b(x2, y2);
        const float r2 = r * r;

        const glm::vec2 v  = b - a;
        const float     c2 = glm::dot(v, v);

        for (int y = minY; y <= maxY; ++y) {
            uint32* row = pixels + y * STRIDE;
            const glm::vec2 rowP(0.0f, static_cast<float>(y));
            for (int x = minX; x <= maxX; ++x) {
                const glm::vec2 w = glm::vec2(x, y) - a;
                const float c1 = glm::dot(w, v);
                float d2;
                if      (c1 <= 0)  d2 = glm::dot(w, w);
                else if (c2 <= c1) d2 = glm::dot(glm::vec2(x,y) - b, glm::vec2(x,y) - b);
                else               d2 = glm::dot(w, w) - (c1 * c1) / c2;

                if (d2 <= r2) {
                    uint32& px = row[x];
    #ifdef ALPHA_BLEND
                    if (color.a == 0xFF)
                        px = static_cast<u32>(color);
                    else
                        px = static_cast<u32>(mixColors(Color(px), color));
    #else
                    px = static_cast<u32>(color);
    #endif
                }
            }
        }
    }
    inline void Canvas::drawLine(const glm::ivec2& p1, const glm::ivec2& p2, const Color color, const float thickness) const {
        drawLine(p1.x, p1.y, p2.x, p2.y, color, thickness);
    }

    inline void Canvas::drawCanvas(const i32 x0, const i32 y0, const i32 width, const i32 height, const Canvas& sourceCanvas) const {
        if (sourceCanvas.getWidth() == 0 || sourceCanvas.getHeight() == 0) return;

        NormalizedRectangle nr{};
        if (!nr.normalizeRectangle(x0, y0, width, height, static_cast<i32>(WIDTH), static_cast<i32>(HEIGHT))) return;

        const i32 xa = (width  < 0) ? nr.ox2 : nr.ox1;
        const i32 ya = (height < 0) ? nr.oy2 : nr.oy1;

        const i32 srcW = static_cast<i32>(sourceCanvas.getWidth());
        const i32 srcH = static_cast<i32>(sourceCanvas.getHeight());

        for (i32 y = nr.y1; y <= nr.y2; ++y) {
            uint32* dstRow = pixels + y * STRIDE;
            u32 ny = static_cast<u32>((y - ya) * srcH / height);
            if (ny >= static_cast<u32>(srcH)) ny = srcH - 1;

            for (i32 x = nr.x1; x <= nr.x2; ++x) {
                u32 nx = static_cast<u32>((x - xa) * srcW / width);
                if (nx >= static_cast<u32>(srcW)) nx = srcW - 1;

                const Color sourcePixel = sourceCanvas.getPixelColor(nx, ny);
                if (sourcePixel.a == 0) continue;

                uint32& px = dstRow[x];
    #ifdef ALPHA_BLEND
                if (sourcePixel.a == 0xFF)
                    px = static_cast<u32>(sourcePixel);
                else
                    px = static_cast<u32>(mixColors(Color(px), sourcePixel));
    #else
                px = static_cast<u32>(sourcePixel);
    #endif
            }
        }
    }
    inline void Canvas::drawCanvas(const glm::ivec2& p, const i32 width, const i32 height, const Canvas& sourceCanvas) const {
        drawCanvas(p.x, p.y, width, height, sourceCanvas);
    }

    inline void Canvas::blitCanvas(const Canvas& sourceCanvas) const {
        const i32 srcW      = static_cast<i32>(sourceCanvas.getWidth());
        const i32 srcH      = static_cast<i32>(sourceCanvas.getHeight());
        const i32 dstW      = static_cast<i32>(WIDTH);
        const i32 dstH      = static_cast<i32>(HEIGHT);
        const i32 srcStride = static_cast<i32>(sourceCanvas.getStride());
        const i32 dstStride = static_cast<i32>(STRIDE);

        const i32 stepX = (srcW << 16) / dstW;
        const i32 stepY = (srcH << 16) / dstH;

        i32 srcY_fp = 0;
        for (i32 y = 0; y < dstH; ++y) {
            const u32* srcRow = sourceCanvas.pixels + (srcY_fp >> 16) * srcStride;
            u32*       dstRow = pixels              +  y              * dstStride;

            i32 srcX_fp = 0;
            for (i32 x = 0; x < dstW; ++x) {
                const Color sourcePixel = Color(srcRow[srcX_fp >> 16]);
                if (sourcePixel.a == 0) { srcX_fp += stepX; ++dstRow; continue; }

                uint32& px = dstRow[x];
    #ifdef ALPHA_BLEND
                if (sourcePixel.a == 0xFF)
                    px = static_cast<u32>(sourcePixel);
                else
                    px = static_cast<u32>(mixColors(Color(px), sourcePixel));
    #else
                px = static_cast<u32>(sourcePixel);
    #endif
                srcX_fp += stepX;
            }
            srcY_fp += stepY;
        }
    }

    inline void Canvas::writeCharBaseline(const char c, const int32 x, const int32 y, const int32 font_size, const Color color) const {
        const uint8* glyph = font8x8_basic[static_cast<uint8>(c)];

        for (int32 py = 0; py < font_size; ++py) {
            // Map output y to glyph row (0..7)
            const int32 glyph_row = (py * 8) / font_size;

            const uint8 bits = glyph[glyph_row];

            for (int32 px = 0; px < font_size; ++px) {
                const int32 draw_x = x + px;
                const int32 draw_y = y - font_size + py;
                if (draw_x < 0 || draw_x >= WIDTH || draw_y < 0 || draw_y >= HEIGHT) continue;

                // Map output x to glyph column (0..7)
                const int32 glyph_col = (px * 8) / font_size;

                if (bits & (1 << glyph_col)) {
                    pixels[getPixelIndex(draw_x, draw_y)] = static_cast<uint32>(color);
                }
            }
        }
    }
    inline void Canvas::writeCharBaseline(const char c, const glm::i32vec2& p, const int32 font_size, const Color color) const {
        writeCharBaseline(c, p.x, p.y, font_size, color);
    }
    inline void Canvas::writeStringBaseline(const std::string& s, const int32 x, const int32 y, const int32 font_size, const Color color) const {
        int32 cursor_x = x;

        for (int i = 0; s[i] != '\0'; ++i) {
            const char c = s[i];
            writeCharBaseline(c, cursor_x, y, font_size, color);
            cursor_x += font_size;
        }
    }
    inline void Canvas::writeStringBaseline(const std::string& s, const glm::i32vec2& p, const int32 font_size, const Color color) const {
        writeStringBaseline(s, p.x, p.y, font_size, color);
    }

    inline bool Canvas::saveToPPM (const std::string& filename) const {
            std::ofstream file(filename, std::ios::out | std::ios::binary);
            if (file.fail()) {
                omni::LOG_ERROR("Could not open file {}", filename);
                return false;
            }

            omni::println(file, "P6\n{} {} 255", WIDTH, HEIGHT);

            for (uint64 i = 0; i < WIDTH * HEIGHT; i++) {
                const uint32 pixel = pixels[i];
                const uint8 bytes[3] = {
                    static_cast<uint8>((pixel >> 0) & 0xff),
                    static_cast<uint8>((pixel >> 8) & 0xff),
                    static_cast<uint8>((pixel >> 16) & 0xff)
                };
                file << bytes[0] << bytes[1] << bytes[2];
            }

            file.close();
            return true;
        }
    inline bool Canvas::saveToJPG (const std::string& filename, const int quality) const {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.fail()) {
            omni::LOG_ERROR("Could not open file {}", filename);
            return false;
        }

        if (stbi_write_jpg(filename.c_str(), static_cast<int32>(WIDTH), static_cast<int32>(HEIGHT), 4, pixels, quality) != 1) {
            omni::LOG_ERROR("Could not write to file {}", filename);
            file.close();
            return false;
        }

        file.close();
        return true;
    }
    inline bool Canvas::saveToPNG (const std::string& filename) const {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.fail()) {
            omni::LOG_ERROR("Could not open file {}", filename);
            return false;
        }

        if (stbi_write_png(filename.c_str(),
            static_cast<int32>(WIDTH),static_cast<int32>(HEIGHT),
            4, pixels, static_cast<int32>(sizeof(pixels[0])*WIDTH)) != 1) {
            omni::LOG_ERROR("Could not write to file {}", filename);
            file.close();
            return false;
        }

        file.close();
        return true;
    }
    inline bool Canvas::saveToBMP (const std::string& filename) const {
        std::ofstream file(filename, std::ios::out | std::ios::binary);
        if (file.fail()) {
            omni::LOG_ERROR("Could not open file {}", filename);
            return false;
        }

        if (stbi_write_bmp(filename.c_str(), static_cast<int32>(WIDTH),static_cast<int32>(HEIGHT), 4, pixels) != 1) {
            omni::LOG_ERROR("Could not write to file {}", filename);
            file.close();
            return false;
        }

        file.close();
        return true;
    }

    inline uint32* Canvas::getPixel      (const uint32 x, const uint32 y) const {
        return &pixels[(y)*STRIDE + (x)];
    }
    inline uint32  Canvas::getPixelIndex (const uint32 x, const uint32 y) const {
        return (y)*STRIDE + (x);
    }
    inline Color   Canvas::getPixelColor (const uint32 x, const uint32 y) const {
        return Color(pixels[(y) * STRIDE + (x)]);
    }

    inline glm::ivec2 normalizedToScreen(const glm::vec2& p, const u32 WIDTH, const u32 HEIGHT) {
        const f32 w  = static_cast<f32>(WIDTH);
        const f32 h  = static_cast<f32>(HEIGHT);
        const f32 s  = glm::min(w, h);
        const f32 cx = w * 0.5f;
        const f32 cy = h * 0.5f;

        return {
            static_cast<i32>(cx + p.x * s * 0.5f),
            static_cast<i32>(cy - p.y * s * 0.5f)
        };
    }

    inline glm::vec2 project(const glm::vec3& p) {
        return glm::vec2(p.x, p.y) / p.z;
    }

    inline glm::vec3 translate(const glm::vec3& p, const glm::vec3& translation) {
        return p + translation;
    }

    // rotation: {pitch, yaw, roll} aka {around x, around y, around z}
    inline glm::vec3 rotate(const glm::vec3& p, const glm::vec3& rotation) {
        const auto rx = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)));
        const auto ry = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)));
        const auto rz = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)));
        return rz * ry * rx * p;
    }

    inline glm::vec3 rotateInverse(const glm::vec3& p, const glm::vec3& rotation) {
        const auto rx = glm::mat3(glm::rotate(glm::mat4(1.0f), -rotation.x, glm::vec3(1, 0, 0)));
        const auto ry = glm::mat3(glm::rotate(glm::mat4(1.0f), -rotation.y, glm::vec3(0, 1, 0)));
        const auto rz = glm::mat3(glm::rotate(glm::mat4(1.0f), -rotation.z, glm::vec3(0, 0, 1)));
        return rx * ry * rz * p;
    }

    inline glm::mat3 makeRotationMatrix(const glm::vec3& rotation) {
        const auto rx = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1, 0, 0)));
        const auto ry = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0, 1, 0)));
        const auto rz = glm::mat3(glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0, 0, 1)));
        return rz * ry * rx;
    }

    inline void Camera::drawObjectWireframe(const Object3D& obj, const Canvas& c, const Color color, const f32 thickness) const {
        const glm::vec3 re_position = obj.position - position;

        const glm::mat3 objRotMatrix = makeRotationMatrix(obj.rotation);
        const glm::mat3 camRotInvMatrix = glm::transpose(makeRotationMatrix(rotation));

        for (const std::vector<FaceIndex>& face : obj.faces) {
            const int faceSize = static_cast<int>(face.size());
            for (int i = 0; i < faceSize; i++) {
                const glm::vec3& p0 = obj.vertices[face[i].vertexIdx];
                const glm::vec3& p1 = obj.vertices[face[(i+1) % faceSize].vertexIdx];

                const glm::vec3 p0CameraRelative = camRotInvMatrix * (objRotMatrix * p0 + re_position);
                const glm::vec3 p1CameraRelative = camRotInvMatrix * (objRotMatrix * p1 + re_position);

                if (p0CameraRelative.z < 0 || p1CameraRelative.z < 0) continue;

                const glm::ivec2 s0 = normalizedToScreen(project(p0CameraRelative), c.getWidth(), c.getHeight());
                const glm::ivec2 s1 = normalizedToScreen(project(p1CameraRelative), c.getWidth(), c.getHeight());
                c.drawLine(s0, s1, color, thickness);
            }
        }
    }

    inline glm::vec3 Camera::directionObj(const glm::vec3& direction) const {
        return makeRotationMatrix({0, rotation.y, 0}) * direction;
    }

    inline glm::vec3 Camera::directionCam(const glm::vec3& direction) const {
        return makeRotationMatrix(rotation) * direction;
    }

    inline glm::vec3 Camera::forwardCam()  const { return makeRotationMatrix(rotation) * Dir3D::FORWARD; }
    inline glm::vec3 Camera::backwardCam() const { return -forwardCam(); }

    inline glm::vec3 Camera::rightCam()  const { return makeRotationMatrix(rotation) * Dir3D::RIGHT; }
    inline glm::vec3 Camera::leftCam()   const { return -rightCam(); }

    inline glm::vec3 Camera::upCam()   const { return makeRotationMatrix(rotation) * Dir3D::UP; }
    inline glm::vec3 Camera::downCam() const { return -upCam(); }

    inline glm::vec3 Camera::forwardObj()  const { return makeRotationMatrix({0, rotation.y, 0}) * Dir3D::FORWARD; }
    inline glm::vec3 Camera::backwardObj() const { return -forwardObj(); }

    inline glm::vec3 Camera::rightObj()  const { return makeRotationMatrix({0, rotation.y, 0}) * Dir3D::RIGHT; }
    inline glm::vec3 Camera::leftObj()   const { return -rightObj(); }

    inline glm::vec3 Camera::upObj()   const { return makeRotationMatrix({0, rotation.y, 0}) * Dir3D::UP; }
    inline glm::vec3 Camera::downObj() const { return -upObj(); }

    inline Object3D loadOBJ(const std::string& fileName) {
        Object3D obj;

        std::ifstream file(fileName);
        if (!file.is_open()) {
            omni::LOG_ERROR("Could not open file {}", fileName);
            return {};
        }

        std::string line;

        while (std::getline(file, line)) {
            std::istringstream ss(line);
            std::string prefix;
            ss >> prefix;

            // ---- Vertex positions ----
            if (prefix == "v") {
                f32vec3 v;
                ss >> v.x >> v.y >> v.z;

                obj.vertices.push_back(v);
            }

            // ---- Vertex normals ----
            if (prefix == "vn") {
                f32vec3 n;
                ss >> n.x >> n.y >> n.z;

                obj.normals.push_back(n);
            }

            // ---- Texture coordinates ----
            else if (prefix == "vt") {
                f32vec2 uv;
                ss >> uv.x >> uv.y;
                obj.texCoords.push_back(uv);
            }

            // ---- Faces ----
            else if (prefix == "f") {
                std::vector<FaceIndex> face;
                std::string vert;

                while (ss >> vert) {
                    // formats:
                    // v
                    // v/vt
                    // v/vt/vn
                    // v//vn

                    int vIndex  = -1;
                    int vnIndex = -1;
                    int vtIndex = -1;

                    size_t s1 = vert.find('/');
                    size_t s2 = std::string::npos;

                    if (s1 == std::string::npos) {
                        vIndex = std::stoi(vert) - 1;
                    } else {
                        vIndex = std::stoi(vert.substr(0, s1)) - 1;

                        s2 = vert.find('/', s1 + 1);

                        std::string vtStr =
                            (s2 == std::string::npos)
                            ? vert.substr(s1 + 1)
                            : vert.substr(s1 + 1, s2 - s1 - 1);

                        if (!vtStr.empty())
                            vtIndex = std::stoi(vtStr) - 1;
                    }

                    face.push_back({
                        vIndex, vtIndex, 0, static_cast<Color>(randomUint32())
                    });
                }

                if (face.size() >= 3)
                    obj.faces.push_back(face);
            }
        }

        return obj;
    }

    inline std::vector<Camera::Vtx> Camera::transformVertices(const Object3D& obj, const Canvas& c) const {
        const glm::vec3 re_position = obj.position - position;
        const glm::mat3 objRot    = makeRotationMatrix(obj.rotation);
        const glm::mat3 camRotInv = glm::transpose(makeRotationMatrix(rotation));

        std::vector<Vtx> vtx(obj.vertices.size());
        for (size_t i = 0; i < obj.vertices.size(); i++) {
            const glm::vec3 v = camRotInv * (objRot * obj.vertices[i] + re_position);

            if (v.z <= 0.001f) continue; // valid defaults to false

            vtx[i].invZ   = 1.0f / v.z;
            vtx[i].screen = normalizedToScreen(project(v), c.getWidth(), c.getHeight());
            vtx[i].valid  = true;
        }
        return vtx;
    }

    inline bool Camera::backfaceCull(const Vtx& v0, const Vtx& v1, const Vtx& v2) {
        const glm::ivec2 e1 = v1.screen - v0.screen;
        const glm::ivec2 e2 = v2.screen - v0.screen;
        return (e1.x * e2.y - e1.y * e2.x) < 0;
    }

    inline void Camera::drawObjectTexture(const Object3D& obj, const Canvas& c, const Canvas* zBuffer, const bool cullBackface) const {
        const auto vtx = transformVertices(obj, c);

        for (const std::vector<FaceIndex>& face : obj.faces) {
            if (face.size() < 3) continue;

            const Vtx& v0 = vtx[face[0].vertexIdx];
            const Vtx& v1 = vtx[face[1].vertexIdx];
            const Vtx& v2 = vtx[face[2].vertexIdx];
            if (!v0.valid || !v1.valid || !v2.valid) continue;

            if (cullBackface && backfaceCull(v0, v1, v2)) continue;

            const glm::vec2 uv0(obj.texCoords[face[0].texCoordIdx].x, obj.texCoords[face[0].texCoordIdx].y);
            const glm::vec2 uv1(obj.texCoords[face[1].texCoordIdx].x, obj.texCoords[face[1].texCoordIdx].y);
            const glm::vec2 uv2(obj.texCoords[face[2].texCoordIdx].x, obj.texCoords[face[2].texCoordIdx].y);

            for (size_t i = 1; i + 1 < face.size(); i++) {
                const Vtx& a = vtx[face[i].vertexIdx];
                const Vtx& b = vtx[face[i + 1].vertexIdx];
                if (!a.valid || !b.valid) continue;

                const glm::vec2 auv(obj.texCoords[face[i].texCoordIdx].x,     obj.texCoords[face[i].texCoordIdx].y);
                const glm::vec2 buv(obj.texCoords[face[i+1].texCoordIdx].x,   obj.texCoords[face[i+1].texCoordIdx].y);

                c.fillTriangleUV(
                    v0.screen, a.screen, b.screen,
                    uv0 * v0.invZ, auv * a.invZ, buv * b.invZ,
                    v0.invZ, a.invZ, b.invZ,
                    *obj.tex, zBuffer
                );
            }
        }
    }
    inline void Camera::drawObjectColor(const Object3D& obj, const Canvas& c, const Canvas* zBuffer, const bool cullBackface) const {
        const auto vtx = transformVertices(obj, c);

        for (const std::vector<FaceIndex>& face : obj.faces) {
            if (face.size() < 3) continue;

            const Vtx& v0 = vtx[face[0].vertexIdx];
            const Vtx& v1 = vtx[face[1].vertexIdx];
            const Vtx& v2 = vtx[face[2].vertexIdx];
            if (!v0.valid || !v1.valid || !v2.valid) continue;

            if (cullBackface && backfaceCull(v0, v1, v2)) continue;

            const Color c0 = face[0].randColor;
            const Color c1 = face[1].randColor;
            const Color c2 = face[2].randColor;

            for (size_t i = 1; i + 1 < face.size(); i++) {
                const Vtx& a = vtx[face[i].vertexIdx];
                const Vtx& b = vtx[face[i + 1].vertexIdx];
                if (!a.valid || !b.valid) continue;

                c.fillTriangle(
                    v0.screen, a.screen, b.screen,
                    c0, face[i].randColor, face[i+1].randColor,
                    v0.invZ, a.invZ, b.invZ,
                    zBuffer
                );
            }
        }
    }
    inline void Camera::drawObjectDepth(const Object3D& obj, const Canvas& c, const Canvas* zBuffer, const bool cullBackface) const {
        const auto vtx = transformVertices(obj, c);

        for (const std::vector<FaceIndex>& face : obj.faces) {
            if (face.size() < 3) continue;

            const Vtx& v0 = vtx[face[0].vertexIdx];
            const Vtx& v1 = vtx[face[1].vertexIdx];
            const Vtx& v2 = vtx[face[2].vertexIdx];
            if (!v0.valid || !v1.valid || !v2.valid) continue;

            if (cullBackface && backfaceCull(v0, v1, v2)) continue;

            auto depthColor = [](float invZ) -> Color {
                const u8 z = static_cast<u8>(glm::clamp(invZ, 0.0f, 1.0f) * 255.0f);
                return {z, z, z, 0xff};
            };

            for (size_t i = 1; i + 1 < face.size(); i++) {
                const Vtx& a = vtx[face[i].vertexIdx];
                const Vtx& b = vtx[face[i + 1].vertexIdx];
                if (!a.valid || !b.valid) continue;

                c.fillTriangle(
                    v0.screen, a.screen, b.screen,
                    depthColor(v0.invZ), depthColor(a.invZ), depthColor(b.invZ),
                    v0.invZ, a.invZ, b.invZ,
                    zBuffer
                );
            }
        }
    }
    inline void Camera::drawObject(const Object3D& obj, const Canvas& c, const Canvas* zBuffer, const bool cullBackface) const {
        if (obj.tex == nullptr)
            drawObjectColor(obj, c, zBuffer, cullBackface);
        else
            drawObjectTexture(obj, c, zBuffer, cullBackface);
    }

#endif

}

#endif //GRAPHITE_H
