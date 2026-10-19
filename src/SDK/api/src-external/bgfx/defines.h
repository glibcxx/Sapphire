#pragma once

#include "pch.h" // IWYU pragma: keep

#define BGFX_BUFFER_NONE UINT16_C(0x0000)
#define BGFX_BUFFER_COMPUTE_READ UINT16_C(0x0100)  //!< Buffer will be read by shader.
#define BGFX_BUFFER_COMPUTE_WRITE UINT16_C(0x0200) //!< Buffer will be used for writing.
#define BGFX_BUFFER_DRAW_INDIRECT UINT16_C(0x0400) //!< Buffer will be used for storing draw indirect commands.
#define BGFX_BUFFER_ALLOW_RESIZE UINT16_C(0x0800)  //!< Allow dynamic index/vertex buffer resize during update.
#define BGFX_BUFFER_INDEX32 UINT16_C(0x1000)       //!< Index buffer contains 32-bit indices.
#define BGFX_BUFFER_COMPUTE_READ_WRITE (0 | BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_COMPUTE_WRITE)

#define BGFX_STATE_WRITE_R UINT64_C(0x0000000000000001) //!< Enable R write.
#define BGFX_STATE_WRITE_G UINT64_C(0x0000000000000002) //!< Enable G write.
#define BGFX_STATE_WRITE_B UINT64_C(0x0000000000000004) //!< Enable B write.
#define BGFX_STATE_WRITE_A UINT64_C(0x0000000000000008) //!< Enable alpha write.
#define BGFX_STATE_WRITE_Z UINT64_C(0x0000004000000000) //!< Enable depth write.
#define BGFX_STATE_WRITE_RGB (0 | BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B)
#define BGFX_STATE_WRITE_MASK (0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z)

#define BGFX_STATE_DEPTH_TEST_LESS UINT64_C(0x0000000000000010)     //!< Enable depth test, less.
#define BGFX_STATE_DEPTH_TEST_LEQUAL UINT64_C(0x0000000000000020)   //!< Enable depth test, less or equal.
#define BGFX_STATE_DEPTH_TEST_EQUAL UINT64_C(0x0000000000000030)    //!< Enable depth test, equal.
#define BGFX_STATE_DEPTH_TEST_GEQUAL UINT64_C(0x0000000000000040)   //!< Enable depth test, greater or equal.
#define BGFX_STATE_DEPTH_TEST_GREATER UINT64_C(0x0000000000000050)  //!< Enable depth test, greater.
#define BGFX_STATE_DEPTH_TEST_NOTEQUAL UINT64_C(0x0000000000000060) //!< Enable depth test, not equal.
#define BGFX_STATE_DEPTH_TEST_NEVER UINT64_C(0x0000000000000070)    //!< Enable depth test, never.
#define BGFX_STATE_DEPTH_TEST_ALWAYS UINT64_C(0x0000000000000080)   //!< Enable depth test, always.
#define BGFX_STATE_DEPTH_TEST_SHIFT 4                               //!< Depth test state bit shift
#define BGFX_STATE_DEPTH_TEST_MASK UINT64_C(0x00000000000000f0)     //!< Depth test state bit mask

#define BGFX_STATE_BLEND_ZERO UINT64_C(0x0000000000001000)          //!< 0, 0, 0, 0
#define BGFX_STATE_BLEND_ONE UINT64_C(0x0000000000002000)           //!< 1, 1, 1, 1
#define BGFX_STATE_BLEND_SRC_COLOR UINT64_C(0x0000000000003000)     //!< Rs, Gs, Bs, As
#define BGFX_STATE_BLEND_INV_SRC_COLOR UINT64_C(0x0000000000004000) //!< 1-Rs, 1-Gs, 1-Bs, 1-As
#define BGFX_STATE_BLEND_SRC_ALPHA UINT64_C(0x0000000000005000)     //!< As, As, As, As
#define BGFX_STATE_BLEND_INV_SRC_ALPHA UINT64_C(0x0000000000006000) //!< 1-As, 1-As, 1-As, 1-As
#define BGFX_STATE_BLEND_DST_ALPHA UINT64_C(0x0000000000007000)     //!< Ad, Ad, Ad, Ad
#define BGFX_STATE_BLEND_INV_DST_ALPHA UINT64_C(0x0000000000008000) //!< 1-Ad, 1-Ad, 1-Ad ,1-Ad
#define BGFX_STATE_BLEND_DST_COLOR UINT64_C(0x0000000000009000)     //!< Rd, Gd, Bd, Ad
#define BGFX_STATE_BLEND_INV_DST_COLOR UINT64_C(0x000000000000a000) //!< 1-Rd, 1-Gd, 1-Bd, 1-Ad
#define BGFX_STATE_BLEND_SRC_ALPHA_SAT UINT64_C(0x000000000000b000) //!< f, f, f, 1; f = min(As, 1-Ad)
#define BGFX_STATE_BLEND_FACTOR UINT64_C(0x000000000000c000)        //!< Blend factor
#define BGFX_STATE_BLEND_INV_FACTOR UINT64_C(0x000000000000d000)    //!< 1-Blend factor
#define BGFX_STATE_BLEND_SHIFT 12                                   //!< Blend state bit shift
#define BGFX_STATE_BLEND_MASK UINT64_C(0x000000000ffff000)          //!< Blend state bit mask

#define BGFX_STATE_BLEND_EQUATION_ADD UINT64_C(0x0000000000000000)    //!< Blend add: src + dst.
#define BGFX_STATE_BLEND_EQUATION_SUB UINT64_C(0x0000000010000000)    //!< Blend subtract: src - dst.
#define BGFX_STATE_BLEND_EQUATION_REVSUB UINT64_C(0x0000000020000000) //!< Blend reverse subtract: dst - src.
#define BGFX_STATE_BLEND_EQUATION_MIN UINT64_C(0x0000000030000000)    //!< Blend min: min(src, dst).
#define BGFX_STATE_BLEND_EQUATION_MAX UINT64_C(0x0000000040000000)    //!< Blend max: max(src, dst).
#define BGFX_STATE_BLEND_EQUATION_SHIFT 28                            //!< Blend equation bit shift
#define BGFX_STATE_BLEND_EQUATION_MASK UINT64_C(0x00000003f0000000)   //!< Blend equation bit mask

#define BGFX_STATE_CULL_CW UINT64_C(0x0000001000000000)   //!< Cull clockwise triangles.
#define BGFX_STATE_CULL_CCW UINT64_C(0x0000002000000000)  //!< Cull counter-clockwise triangles.
#define BGFX_STATE_CULL_SHIFT 36                          //!< Culling mode bit shift
#define BGFX_STATE_CULL_MASK UINT64_C(0x0000003000000000) //!< Culling mode bit mask

#define BGFX_STATE_PT_TRISTRIP UINT64_C(0x0001000000000000)  //!< Tristrip.
#define BGFX_STATE_PT_LINES UINT64_C(0x0002000000000000)     //!< Lines.
#define BGFX_STATE_PT_LINESTRIP UINT64_C(0x0003000000000000) //!< Line strip.
#define BGFX_STATE_PT_POINTS UINT64_C(0x0004000000000000)    //!< Points.
#define BGFX_STATE_PT_SHIFT 48                               //!< Primitive type bit shift
#define BGFX_STATE_PT_MASK UINT64_C(0x0007000000000000)      //!< Primitive type bit mask