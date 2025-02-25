/*-*- mode:c;indent-tabs-mode:nil;c-basic-offset:2;tab-width:8;coding:utf-8 -*-│
│vi: set net ft=c ts=2 sts=2 sw=2 fenc=utf-8                                :vi│
╞══════════════════════════════════════════════════════════════════════════════╡
│ Copyright 2022 Justine Alexandra Roberts Tunney                              │
│                                                                              │
│ Permission to use, copy, modify, and/or distribute this software for         │
│ any purpose with or without fee is hereby granted, provided that the         │
│ above copyright notice and this permission notice appear in all copies.      │
│                                                                              │
│ THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL                │
│ WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED                │
│ WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE             │
│ AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL         │
│ DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR        │
│ PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER               │
│ TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR             │
│ PERFORMANCE OF THIS SOFTWARE.                                                │
╚─────────────────────────────────────────────────────────────────────────────*/
#include "blink/assert.h"
#include "blink/endian.h"
#include "blink/machine.h"

void OpCpuid(P) {
  u32 ax, bx, cx, dx, jit;
  ax = bx = cx = dx = 0;
  switch (Get32(m->ax)) {
    case 0:
    case 0x80000000:
      unassert(m->system->brand);
      ax = 7;
      bx = Read32((u8 *)(m->system->brand + 0));
      dx = Read32((u8 *)(m->system->brand + 4));
      cx = Read32((u8 *)(m->system->brand + 8));
      break;
    case 1:
      cx |= 1 << 0;   // sse3
      cx |= 1 << 1;   // pclmulqdq
      cx |= 1 << 9;   // ssse3
      cx |= 1 << 23;  // popcnt
      cx |= 1 << 30;  // rdrnd
      cx |= 0 << 25;  // aes
      cx |= 1 << 13;  // cmpxchg16b
      dx |= 1 << 0;   // fpu
      dx |= 1 << 4;   // tsc
      dx |= 1 << 6;   // pae
      dx |= 1 << 8;   // cmpxchg8b
      dx |= 1 << 15;  // cmov
      dx |= 1 << 19;  // clflush
      dx |= 1 << 23;  // mmx
      dx |= 1 << 24;  // fxsave
      dx |= 1 << 25;  // sse
      dx |= 1 << 26;  // sse2
      cx |= 0 << 19;  // sse4.1
      cx |= 0 << 20;  // sse4.2
      break;
    case 7:
      switch (Get32(m->cx)) {
        case 0:
          bx |= 1 << 0;   // fsgsbase
          bx |= 1 << 8;   // bmi2
          bx |= 1 << 9;   // erms
          bx |= 1 << 18;  // rdseed
          bx |= 1 << 19;  // adx
          cx |= 1 << 22;  // rdpid
          break;
        default:
          break;
      }
      break;
    case 0x80000001:
      jit = !IsJitDisabled(&m->system->jit);
      cx |= 1 << 0;     // lahf
      cx |= jit << 31;  // jit
      dx |= 1 << 0;     // fpu
      dx |= 1 << 8;     // cmpxchg8b
      dx |= 1 << 11;    // syscall
      dx |= 1 << 15;    // cmov
      dx |= 1 << 23;    // mmx
      dx |= 1 << 24;    // fxsave
      dx |= 1 << 27;    // rdtscp
      dx |= 1 << 29;    // long
      break;
    case 0x80000007:
      dx |= 1 << 8;  // invtsc
      break;
    default:
      break;
  }
  Put64(m->ax, ax);
  Put64(m->bx, bx);
  Put64(m->cx, cx);
  Put64(m->dx, dx);
}
