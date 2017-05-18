// See LICENSE file for copyright and license details
#include "internal.h"

enum {
	UTF8_H1 = 0x00 << 7, // 1-byte header 0xxxxxxx.
	UTF8_H2 = 0x06 << 5, // 2-byte header 110xxxxx.
	UTF8_H3 = 0x0e << 4, // 3-byte header 1110xxxx.
	UTF8_H4 = 0x1e << 3, // 4-byte header 11110xxx.
	UTF8_HC = 0x02 << 6, // Continuation byte header 10xxxxxx.
};

#include <stdio.h>

size_t
stxutf8len(const spx sp)
{
	size_t n = 0;
	size_t i = 0;
	while (i < sp.len) {
		unsigned char ch = sp.mem[i];
		if (0x0 == (sp.mem[i] >> 7)) {
			i += 1;
		} else if (0x06 == (ch >> 5)) {
			i += 2;
		} else if (0x0e == (ch >> 4)) {
			i += 3;
		} else if (0x1e == (ch >> 3)) {
			i += 4;
		} else {
			// Error condition.
			return 0;
		}
		++n;
	}

	return n;
}

size_t
stxutf8n32(uint32_t wc)
{
	size_t len;
	if (wc < 0x000080) {
		len = 1;
	} else if (wc < 0x00800) {
		len = 2;
	} else if (wc < 0x10000) {
		len = 3;
	} else if (wc < 0x11000) {
		len = 4;
	} else {
		// Error case.
		len = 0;
	}

	return len;
}

size_t
stxutf8f32(void *dest, uint32_t wc)
{
	size_t n = stxutf8n32(wc);
	uint8_t header;
	switch (n) {
	case 1:
		header = UTF8_H1;
		break;
	case 2:
		header = UTF8_H2;
		break;
	case 3:
		header = UTF8_H3;
		break;
	case 4:
		header = UTF8_H4;
		break;
	default:
		// Error case.
		return n;
		break;
	}

	for (int i = n-1; i > 0; --i) {
		((uint8_t *)dest)[i] = (UTF8_HC | (wc & 0x3F));
		wc >>= 6;
	}

	((uint8_t *)dest)[0] = (header | wc);

	return n;
}
