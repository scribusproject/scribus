/*
 * "$Id$"
 *
 *   RC4 functions for HTMLDOC.
 *
 *   Original code by Rob Earhart
 *   Copyright 1999 by Carnegie Mellon University, All Rights Reserved
 *
 *   Permission to use, copy, modify, and distribute this software and its
 *   documentation for any purpose and without fee is hereby granted,
 *   provided that the above copyright notice appear in all copies and that
 *   both that copyright notice and this permission notice appear in
 *   supporting documentation, and that the name of Carnegie Mellon
 *   University not be used in advertising or publicity pertaining to
 *   distribution of the software without specific, written prior
 *   permission.
 *
 *   CARNEGIE MELLON UNIVERSITY DISCLAIMS ALL WARRANTIES WITH REGARD TO
 *   THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND
 *   FITNESS, IN NO EVENT SHALL CARNEGIE MELLON UNIVERSITY BE LIABLE FOR
 *   ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *   WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *   ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 *   OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _RC4_H_
#  define _RC4_H_

#  ifdef __cplusplus
extern "C" {
#  endif /* __cplusplus */


/*
 * RC4 context...
 */

typedef struct
{
  unsigned char	sbox[256];	/* S boxes for encryption */
  int		i, j;		/* Current indices into S boxes */
} rc4_context_t;


/*
 * Prototypes...
 */

extern void	rc4_init(rc4_context_t *context, const unsigned char *key,
			 unsigned keylen);
extern void	rc4_encrypt(rc4_context_t *context, const unsigned char *input,
			    unsigned char *output, unsigned len);

#  ifdef __cplusplus
}
#  endif /* __cplusplus */

#endif /* !_RC4_H_ */

/*
 * End of "$Id$".
 */
