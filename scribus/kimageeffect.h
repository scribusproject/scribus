/* This file is part of the KDE libraries
 Copyright (C) 1998, 1999, 2001, 2002 Daniel M. Duley <mosfet@interaccess.com>
 (C) 1998, 1999 Christian Tibirna <ctibirna@total.net>
 (C) 1998, 1999 Dirk A. Mueller <mueller@kde.org>

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

// $Id$

#ifndef __KIMAGE_EFFECT_H
#define __KIMAGE_EFFECT_H

class QImage;
class QSize;
class QColor;

/**
 * This class includes various @ref QImage based graphical effects.
 *
 * Everything is
 * static, so there is no need to create an instance of this class. You can
 * just call the static methods. They are encapsulated here merely to provide
 * a common namespace.
 */

class KImageEffect
{
public:
    enum GradientType { VerticalGradient, HorizontalGradient,
                        DiagonalGradient, CrossDiagonalGradient,
                        PyramidGradient, RectangleGradient,
                        PipeCrossGradient, EllipticGradient };
    enum RGBComponent { Red, Green, Blue, Gray, All };

    enum Lighting {NorthLite, NWLite, WestLite, SWLite,
                   SouthLite, SELite, EastLite, NELite};

    enum ModulationType { Intensity, Saturation, HueShift, Contrast };

    enum NoiseType { UniformNoise=0, GaussianNoise, MultiplicativeGaussianNoise,
                     ImpulseNoise, LaplacianNoise, PoissonNoise};

    enum RotateDirection{ Rotate90, Rotate180, Rotate270 };

    /**
     * Create a gradient from color a to color b of the specified type.
     *
     * @param size The desired size of the gradient.
     * @param ca Color a
     * @param cb Color b
     * @param type The type of gradient.
     * @param ncols The number of colors to use when not running on a
     * truecolor display. The gradient will be dithered to this number of
     * colors. Pass 0 to prevent dithering.
     */
    static QImage gradient(const QSize &size, const QColor &ca,
                           const QColor &cb, GradientType type, int ncols=3);

    /**
     * Create an unbalanced gradient.

     * An unbalanced gradient is a gradient where the transition from
     * color a to color b is not linear, but in this case, exponential.
     *
     * @param size The desired size of the gradient.
     * @param ca Color a
     * @param cb Color b
     * @param type The type of gradient.
     * @param xfactor The x decay length. Use a value between -200 and 200.
     * @param yfactor The y decay length.
     * @param ncols The number of colors. See KPixmapEffect:gradient.
     */
    static QImage unbalancedGradient(const QSize &size, const QColor &ca,
	    const QColor &cb, GradientType type, int xfactor = 100,
	    int yfactor = 100, int ncols = 3);

    /**
     * Blends a color into the destination image, using an opacity
     * value for blending one into another. Very fast direct pixel
     * manipulation is used.
     *
     * @author Karol Szwed (gallium@kde.org)
     * @param clr source color to be blended into the destination image.
     * @param dst destination image in which the source will be blended into.
     * @param opacity opacity (in percent) which determines how much the source
     *                color will be blended into the destination image.
     * @return The destination image (dst) containing the result.
     */
    static QImage& blend(const QColor& clr, QImage& dst, float opacity);

    /**
     * Blend the src image into the destination image, using an opacity
     * value for blending one into another. Very fast direct pixel
     * manipulation is used.
     *
     * @author Karol Szwed (gallium@kde.org)
     * @param src source image to be blended into the destination image.
     * @param dst destination image in which the source will be blended into.
     * @param opacity opacity (in percent) which determines how much the source
     *                image will be blended into the destination image.
     * @return The destination image (dst) containing the result.
     */
    static QImage& blend(QImage& src, QImage& dst, float opacity);

     /**
     * Blend the provided image into a background of the indicated color.
     *
     * @param initial_intensity this parameter takes values from -1 to 1:
     *              a) if positive: how much to fade the image in its
     *                              less affected spot
     *              b) if negative: roughly indicates how much of the image
     *                              remains unaffected
     * @param bgnd indicates the color of the background to blend in
     * @param eff lets you choose what kind of blending you like
     * @param anti_dir blend in the opposite direction (makes no much sense
     *                  with concentric blending effects)
     * @param image must be 32bpp
     */
    static QImage& blend(QImage &image, float initial_intensity,
                      const QColor &bgnd, GradientType eff,
                      bool anti_dir=false);

    /**
     * Blend an image into another one, using a gradient type
     * for blending from one to another.
     *
     * @param image1 source1 and result of blending
     * @param image2 source2 of blending
     * @param gt gradient type for blending between source1 and source2
     * @param xf x decay length for unbalanced gradient tpye
     * @param yf y decay length for unbalanced gradient tpye
     */
    static QImage& blend(QImage &image1,QImage &image2,
			 GradientType gt, int xf=100, int yf=100);

    /**
     * Blend an image into another one, using a color channel of a
     * third image for the decision of blending from one to another.
     *
     * @param image1 Source 1 and result of blending
     * @param image2 Source 2 of blending
     * @param blendImage If the gray value of of pixel is 0, the result
     *               for this pixel is that of image1; for a gray value
     *               of 1, the pixel of image2 is used; for a value
     *               inbetween, a corresponding blending is used.
     * @param channel The RBG channel to use for the blending decision.
     */
    static QImage& blend(QImage &image1, QImage &image2,
			 QImage &blendImage, RGBComponent channel);

    /**
     * Blend an image into another one, using alpha in the expected way.
     * @author Rik Hemsley (rikkus) <rik@kde.org>
     */
    static bool blend(const QImage & upper, const QImage & lower, QImage & output);
// Not yet...    static bool blend(const QImage & image1, const QImage & image2, QImage & output, const QRect & destRect);

    /**
     * Blend an image into another one, using alpha in the expected way and
     * over coordinates @p x and @p y with respect to the lower image.
     * The output is a QImage which is the @p upper image already blended
     * with the @p lower one, so its size will be (in general) the same than
     * @p upper instead of the same size than @p lower like the method above.
     * In fact, the size of @p output is like upper's one only when it can be
     * painted on lower, if there has to be some clipping, output's size will
     * be the clipped area and x and y will be set to the correct up-left corner
     * where the clipped rectangle begins.
     */
    static bool blend(int &x, int &y, const QImage & upper, const QImage & lower, QImage & output);
    /**
     * Blend an image into another one, using alpha in the expected way and
     * over coordinates @p x and @p y with respect to the lower image.
     * The output is painted in the own @p lower image. This is an optimization
     * of the @ref blend method above provided by convenience.
     */
    static bool blendOnLower(int x, int y, const QImage & upper, const QImage & lower);

    /**
     * Modifies the intensity of a pixmap's RGB channel component.
     *
     * @author Daniel M. Duley (mosfet)
     * @param image The QImage to process.
     * @param percent Percent value. Use a negative value to dim.
     * @param channel Which channel(s) should be modified
     * @return The @p image, provided for convenience.
     */
    static QImage& channelIntensity(QImage &image, float percent,
                                    RGBComponent channel);

    /**
     * Fade an image to a certain background color.
     *
     * The number of colors will not be changed.
     *
     * @param image The QImage to process.
     * @param val The strength of the effect. 0 <= val <= 1.
     * @param color The background color.
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& fade(QImage &img, float val, const QColor &color);


    /**
     * This recolors a pixmap. The most dark color will become color a,
     * the most bright one color b, and in between.
     *
     * @param image A QImage to process.
     * @param ca Color a
     * @param cb Color b
     */
    static QImage& flatten(QImage &image, const QColor &ca,
           const QColor &cb, int ncols=0);

    /**
     * Build a hash on any given @ref QImage
     *
     * @param image The QImage to process
     * @param lite The hash faces the indicated lighting (cardinal poles).
     * @param spacing How many unmodified pixels inbetween hashes.
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& hash(QImage &image, Lighting lite=NorthLite,
                        unsigned int spacing=0);

    /**
     * Either brighten or dim the image by a specified percent.
     * For example, .50 will modify the colors by 50%.
     *
     * @author Daniel M. Duley (mosfet)
     * @param image The QImage to process.
     * @param percent The percent value. Use a negative value to dim.
     * @return Returns The @ref image(), provided for convenience.
     */
    static QImage& intensity(QImage &image, float percent);

    /**
     * Modulate the image with a color channel of another image.
     *
     * @param image The QImage to modulate and result.
     * @param modImage The QImage to use for modulation.
     * @param reverse Invert the meaning of image/modImage; result is image!
     * @param type The modulation Type to use.
     * @param factor The modulation amplitude; with 0 no effect [-200;200].
     * @param channel The RBG channel of image2 to use for modulation.
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& modulate(QImage &image, QImage &modImage, bool reverse,
		ModulationType type, int factor, RGBComponent channel);

    /**
     * Convert an image to grayscale.
     *
     * @author Daniel M. Duley (mosfet)
     * @param image The @ref QImage to process.
     * @param fast Set to @p true in order to use a faster but non-photographic
     * quality algorithm. Appropriate for things such as toolbar icons.
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& toGray(QImage &image, bool fast = false);

    /**
     * Desaturate an image evenly.
     *
     * @param image The QImage to process.
     * @param desat A value between 0 and 1 setting the degree of desaturation
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& desaturate(QImage &image, float desat = 0.3);

    /**
     * Fast, but low quality contrast of an image. Also see contrastHSV.
     *
     * @author Daniel M. Duley (mosfet)
     * @param image The QImage to process.
     * @param c A contrast value between -255 to 255.
     * @return The @ref image(), provided for convenience.
     */
    static QImage& contrast(QImage &image, int c);

    /**
     * Dither an image using Floyd-Steinberg dithering for low-color
     * situations.
     *
     * @param image The QImage to process.
     * @param palette The color palette to use
     * @param size The size of the palette
     * @return Returns the @ref image(), provided for convenience.
     */
    static QImage& dither(QImage &img, const QColor *palette, int size);

    /**
     * Calculate the image for a selected image, for instance a selected icon
     * on the desktop.
     * @param img the QImage to select
     * @param col the selected color, usually from QColorGroup::highlight().
     */
    static QImage& selectedImage( QImage &img, const QColor &col );

    /**
     * High quality, expensive HSV contrast. You can do a faster one by just
     * taking a intensity threshold (ie: 128) and incrementing RGB color
     * channels above it and decrementing those below it, but this gives much
     * better results.
     *
     * @author Daniel M. Duley (mosfet)
     * @param img The QImage to process.
     * @param sharpen If true sharpness is increase, (spiffed). Otherwise
     * it is decreased, (dulled).
     */
    static void contrastHSV(QImage &img, bool sharpen=true);

    /**
     * Normalizes the pixel values to span the full range of color values.
     * This is a contrast enhancement technique.
     * @author Daniel M. Duley (mosfet)
     */
    static void normalize(QImage &img);

    /**
     * Performs histogram equalization on the reference
     * image.
     * @author Daniel M. Duley (mosfet)
     */
    static void equalize(QImage &img);

    /**
     * Thresholds the reference image. You can also threshold images by using
     * ThresholdDither in the various QPixmap/QImage convert methods, but this
     * lets you specify a threshold value.
     *
     * @author Daniel M. Duley (mosfet)
     * @param img The QImage to process.
     * @param value The threshold value.
     */
    static void threshold(QImage &img, unsigned int value=128);

    /**
     * Produces a 'solarization' effect seen when exposing a photographic
     * film to light during the development process.
     *
     * @author Daniel M. Duley (mosfet)
     * @param img The QImage to process.
     * @param factor The extent of the solarization (0-99.9)
     */
    static void solarize(QImage &img, double factor=50.0);

    /**
     * Embosses the source image. This involves highlighting the edges
     * and applying various other enhancements in order to get a metal
     * effect.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @return The embossed image. The original is not changed.
     */
    static QImage emboss(QImage &src);

    /**
     * Minimizes speckle noise in the source image using the 8 hull
     * algorithm.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @return The despeckled image. The original is not changed.
     */
    static QImage despeckle(QImage &src);

    /**
     * Produces a neat little "charcoal" effect.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param factor The factor for detecting lines (0-99.0).
     * @return The charcoal image. The original is not changed.
     */
    static QImage charcoal(QImage &src, double factor=50.0);

    /**
     * Rotates the image by the specified amount
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param r The rotate direction.
     * @return The rotated image. The original is not changed.
     */
    static QImage rotate(QImage &src, RotateDirection r);

    /**
     * Scales an image using simple pixel sampling. This does not produce
     * nearly as nice a result as QImage::smoothScale(), but has the
     * advantage of being much faster - only a few milliseconds.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param w The new width.
     * @param h The new height.
     * @return The scaled image. The original is not changed.
     */
    static QImage sample(QImage &src, int w, int h);

    /**
     * Adds noise to an image.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param type The algorithm used to generate the noise.
     * @return The image with noise added. The original is not changed.
     */
    static QImage addNoise(QImage &src, NoiseType type = GaussianNoise);

    /**
     * Blurs an image by convolving pixel neighborhoods.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param factor The percent weight to give to the center pixel.
     * @return The blurred image. The original is not changed.
     */
    static QImage blur(QImage &src, double factor=50.0);

    /**
     * Detects edges in an image using pixel neighborhoods and an edge
     * detection mask.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param factor The percent weight to give to the center pixel.
     * @return The image with edges detected. The original is not changed.
     */
    static QImage edge(QImage &src, double factor=50.0);

    /**
     * Implodes an image by a specified percent.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param factor The extent of the implosion.
     * @param background An RGBA value to use for the background. After the
     * effect some pixels may be "empty". This value is used for those pixels.
     * @return The imploded image. The original is not changed.
     */
    static QImage implode(QImage &src, double factor=30.0,
                   unsigned int background = 0xFFFFFFFF);
    /**
     * Produces an oil painting effect.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param radius The radius of the pixel neighborhood used in applying the
     * effect.
     * @return The new image. The original is not changed.
     */
    static QImage oilPaint(QImage &src, int radius=3);

    /**
     * Sharpens the pixels in the image using pixel neighborhoods.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param factor The percent weight to give to the center pixel.
     * @return The sharpened image. The original is not changed.
     */
    static QImage sharpen(QImage &src, double factor=30.0);

    /**
     * Randomly displaces pixels.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param amount The vicinity for choosing a random pixel to swap.
     * @return The image with pixels displaced. The original is not changed.
     */
    static QImage spread(QImage &src, unsigned int amount=3);

    /**
     * Shades the image using a distance light source.
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param color_shading If true do color shading, otherwise do grayscale.
     * @param azimuth Determines the light source and direction.
     * @param elevation Determines the light source and direction.
     * @return The shaded image. The original is not changed.
     */
    static QImage shade(QImage &src, bool color_shading=true, double azimuth=30.0,
                        double elevation=30.0);
    /**
     * Swirls the image by a specified amount
     *
     * @author Daniel M. Duley (mosfet)
     * @param src The QImage to process.
     * @param degrees The tightness of the swirl.
     * @param background An RGBA value to use for the background. After the
     * effect some pixels may be "empty". This value is used for those pixels.
     * @return The swirled image. The original is not changed.
     */
     static QImage swirl(QImage &src, double degrees=50.0, unsigned int background =
                         0xFFFFFFFF);

     /**
      * Modifies the pixels along a sine wave.
      *
      * @author Daniel M. Duley (mosfet)
      * @param src The QImage to process.
      * @param amplitude The amplitude of the sine wave.
      * @param wavelength The frequency of the sine wave.
      * @return The new image. The original is not changed.
      */
     static QImage wave(QImage &src, double amplitude=25.0, double frequency=150.0,
                        unsigned int background = 0xFFFFFFFF);

private:

    /**
     * Helper function to fast calc some altered (lighten, shaded) colors
     *
     */
    static unsigned int lHash(unsigned int c);
    static unsigned int uHash(unsigned int c);

    /**
     * Helper function to find the nearest color to the RBG triplet
     */
    static int nearestColor( int r, int g, int b, const QColor *pal, int size );

    static void hull(const int x_offset, const int y_offset, const int polarity,
                     const int width, const int height,
                     unsigned int *f, unsigned int *g);
    static unsigned int generateNoise(unsigned int pixel, NoiseType type);
    static unsigned int interpolateColor(QImage *image, double x, double y,
                                         unsigned int background);
};

#endif
