#include <assert.h>
#include <string.h>

#include "glsl_swizzle.h"

static void test_hcr_pixelate_shader_identifiers_remain_distinct(void) {
    char source[] =
        "float t=v_texCoord.t;"
        "float s=v_texCoord.s;"
        "float x=qs*floor(s/qs)+xOffset;"
        "float y=qt*floor(t/qt);"
        "vec2 texc=vec2(x,y);";
    const char expected[] =
        "float t=v_texCoord.y;"
        "float s=v_texCoord.x;"
        "float x=qs*floor(s/qs)+xOffset;"
        "float y=qt*floor(t/qt);"
        "vec2 texc=vec2(x,y);";

    glsl_translate_st_swizzles(source);
    assert(strcmp(source, expected) == 0);
}

static void test_only_supported_member_swizzles_are_translated(void) {
    char source[] =
        "a.s+a.t+a.st+a.ts+a.ss+a.tt+"
        "a.sts+a.texture+s+t+st+ts+ss+tt+"
        "left . s+right.\n tt;";
    const char expected[] =
        "a.x+a.y+a.xy+a.yx+a.xx+a.yy+"
        "a.sts+a.texture+s+t+st+ts+ss+tt+"
        "left . x+right.\n yy;";

    glsl_translate_st_swizzles(source);
    assert(strcmp(source, expected) == 0);
}

int main(void) {
    test_hcr_pixelate_shader_identifiers_remain_distinct();
    test_only_supported_member_swizzles_are_translated();
}
