%
% psi.prolog
%
% Copyright (C) 1996-2000 by vhf computer GmbH + vhf interservice GmbH
% Author:   Georg Fleischmann
%
% created:  2000-09-09
% modified: 
%

%%% this file also contains a section taken from GPL Ghostscript 8.50 "traceimg.ps", see below

% this PostScript code writes a file with the structure below.
% # # # # l		(x0 y0 x1 y1) line
% # # # # # # # #c	(x0 y0 x1 y1 x2 y2 x3 y3) curve
% # w			(width) width
% # # # # # co		(c m y k a) color
% n			new list
% f			list is a filled polygon
% s			list is a group
% cl			list is a clip list (clip with old clip list and use it)
% gs			save current clip list and width to top of stack
% gr			use last clip list (on top of stack) and width
% # # # # # # # # # #	(x y a b c d e f text font) text
%
% Adapted for Scribus by Franz Schmid 15.05.2004
% Also removed the hardcoded Output Filename
% and changed it in a way the -sOutputFile Option of Ghostscript can be used
% Speeded up the flattening of Text by removing unneeded calculations.
% Changed the Output slightly to ease parsing
% m                 moveto  
% l # # # #         (x0 y0 x1 y1) line
% c # # # # # # # # (x0 y0 x1 y1 x2 y2 x3 y3) curve
% w #               (width) Linewidth
% co # # # # #      (c m y k a) color
% n                 new list
% f                 list is a filled polygon
% s                 list is a stroke path
% cp                close current subpath
% ci                list is a clip list (clip with old clip list and use it)
% sp                end of page
% lj #              linejoin
% lc #              linecap
% ld # # #n         linedash count offset d1, d2, dx
% im #              image <name>
% 15.05.2004 Added the Glyphshow Operator.
% 17.05.2004 Made clipping working.
% 20.05.2004 kshow is working now.
% 22.02.2006 added image and colorimage ops

/cfile TraceFile (w) file def
/print { cfile exch writestring } bind def

% whether we have to flatten the text
/flattenText 1 def

% is a Clipping there
/clipCnt 0 def

% remember the current point
/currentX 0 def
/currentY 0 def

% 1st point of path to close the path
/beginX 0 def
/beginY 0 def

% dummy for converting strings
/i_str 50 string def

% 0 = mirror at
/mirror 0 def

% mirror a at 0
/mir
{
	mirror 0 ne
	{	0 exch sub
	}if
} bind def

% scale
/m_a 1 def
/m_b 0 def
/m_c 0 def
/m_d 1 def
/m_x 0 def
/m_y 0 def
/matrix_x	% x y
{
	% ax + cy + tx
	m_c mul exch m_a mul add m_x add
} bind def
/matrix_y	% x y
{
	% bx + dy + ty
	m_d mul exch m_b mul add m_y add
} bind def

/writecurrentcolor
{
	currentcolorspace 0 get /CIEBasedABC eq
	{	currentcolor setrgbcolor
	} if
	currentcolorspace 0 get /DeviceRGB eq
	{
		currentrgbcolor	% -> r g b
		(corgb )print
		2 index i_str cvs print
		( ) print
		1 index i_str cvs print
		( ) print
		i_str cvs print
		( ) print
		pop pop
		.currentopacityalpha	% a
		i_str cvs print
		(\n) print
	} 
	{
		currentcmykcolor	% -> c m y k
		(co )print
		3 index i_str cvs print
		( ) print
		2 index i_str cvs print
		( ) print
		1 index i_str cvs print
		( ) print
		i_str cvs print
		( ) print
		pop pop pop
		.currentopacityalpha	% a
		i_str cvs print
		(\n) print
	} ifelse

} bind def

/writecurrentlinecap
{
	(lc ) print
	currentlinecap i_str cvs print
	(\n) print
} bind def

/writecurrentlinejoin
{
	(lj ) print
	currentlinejoin i_str cvs print
	(\n) print
} bind def

/writecurrentdash
{
	(ld ) print
	currentdash 1 index length i_str cvs print ( ) print i_str cvs print ( ) print
	0 1 2 index length 1 sub
	{
		1 index exch get i_str cvs print ( ) print
	} for
	pop
	(\n) print
} bind def

/writecurrentlinewidth
{
	userdict begin
	currentlinewidth	% w
	storeMatrix

	% (wb + wd + wa + wc) / 2
	dup dup dup m_b abs mul exch m_d abs mul add  exch m_a abs mul add  exch m_c abs mul add  2 div  abs
	(w ) print
	i_str cvs print
	(\n) print
	end
} bind def

/_move	% x y
{
	userdict begin
	(m\n) print
	/currentY exch def
	/currentX exch def
	/beginX currentX def
	/beginY currentY def
	end
} bind def

/_line
{
	userdict begin
	/y1 exch def
	/x1 exch def

	% x x1 ne y y1 ne or
	currentX x1 sub abs 0.001 gt  currentY y1 sub abs 0.001 gt or
	{
		(l ) print
		currentX currentY matrix_x i_str cvs print
		( ) print
		currentX currentY matrix_y i_str cvs print
		( ) print

		x1 y1 matrix_x i_str cvs print
		( ) print
		x1 y1 matrix_y i_str cvs print
		(\n) print
		/currentX x1 def
		/currentY y1 def
	}if
	end
} bind def

/_curve
{
	userdict begin
	% x1 y1 x2 y2 x3 y3
	(c ) print
	currentX currentY matrix_x i_str cvs print
	( ) print
	currentX currentY matrix_y i_str cvs print
	( ) print
	5 index 5 index matrix_x i_str cvs print
	( ) print
	5 index 5 index matrix_y i_str cvs print
	( ) print
	3 index 3 index matrix_x i_str cvs print
	( ) print
	3 index 3 index matrix_y i_str cvs print
	( ) print

	/currentY exch def
	/currentX exch def

	currentX currentY matrix_x i_str cvs print
	( ) print
	currentX currentY matrix_y i_str cvs print
	(\n)print
	pop pop pop pop
	end
} bind def

% modified: 18.10.96
/_close
{
	(cp\n) print
	beginX beginY _line
} bind def

/storeMatrix
{
	userdict begin
	matrix currentmatrix
	0 get /m_a exch def
	matrix currentmatrix
	1 get /m_b exch def
	matrix currentmatrix
	2 get /m_c exch def
	matrix currentmatrix
	3 get /m_d exch def
	matrix currentmatrix
	4 get /m_x exch def
	matrix currentmatrix
	5 get /m_y exch def
	end
} bind def

/rectfill
{
	userdict begin
	(n\n)print			% start polygon
	writecurrentcolor
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
	storeMatrix

	% x y width height
	dup type /arraytype ne
	{
		/hr exch def
		/wr exch def
		/yr exch def
		/xr exch def
		xr yr _move
		xr wr add yr _line
		xr wr add yr hr add _line
		xr yr hr add _line
		xr yr _line
	}
	% numarray
	% numstring
	{
		/ar exch def
		0 4 ar length 1 sub
		{
			/n exch def
			ar n get /xr exch def
			ar n 1 add get /yr exch def
			ar n 2 add get /wr exch def
			ar n 3 add get /hr exch def
			xr yr _move
			xr wr add yr _line
			xr wr add yr hr add _line
			xr yr hr add _line
			xr yr _line
		} for
	}ifelse

	(f\n)print			% close polygon
	end
} bind def

/rectstroke
{
	userdict begin
	(n\n)print			% start rect
	writecurrentcolor
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
	storeMatrix

	% x y width height
	dup type /arraytype ne
	{
		/hr exch def
		/wr exch def
		/yr exch def
		/xr exch def
		xr yr _move
		xr wr add yr _line
		xr wr add yr hr add _line
		xr yr hr add _line
		xr yr _line
	}
	% numarray
	% numstring
	{
		/ar exch def
		0 4 ar length 1 sub
		{
			/n exch def
			ar n get /xr exch def
			ar n 1 add get /yr exch def
			ar n 2 add get /wr exch def
			ar n 3 add get /hr exch def
			xr yr _move
			xr wr add yr _line
			xr wr add yr hr add _line
			xr yr hr add _line
			xr yr _line
		} for
	}ifelse
	(n\n)print			% stroke rect
	end
} bind def

/stroke
{
	(n\n) print
	writecurrentcolor
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
	clipCnt 1 eq
		{ clipsave clip newpath clippath cliprestore } if
	storeMatrix
	{_move} {_line} {_curve} {_close} pathforall
	(s\n)print			% stroke path
	newpath
} bind def

/eofill
{
	(n\n) print			% start polygon
	writecurrentcolor	% write color
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
	clipCnt 1 eq
		{ clipsave clip newpath clippath cliprestore } if
	storeMatrix			% take transformation, scaling, rotation from PostScript
	{_move} {_line} {_curve} {_close} pathforall
	(f\n)print			% close polygon

	newpath				% clear stack
} bind def

/fill
{
	eofill
} bind def

/clip
{
	userdict begin
	(n\n)print			% start clip polygon

	storeMatrix			% take transformation, scaling, rotation from PostScript
	{_move} {_line} {_curve} {_close} pathforall

	(ci\n)print			% close clip polygon begin path
						% we have to close the path!!
	clip
	/clipCnt 1 def
	newpath				% clear stack
	end
} bind def

% we don't clip
% because this doesn't work for flattening text (show, charpath) with NeXT PostScript Code
/rectclip
{
	% let Scribus decide what to do with rci: pop pop pop pop
	(rci ) print 
	i_str cvs print ( ) print
	i_str cvs print ( ) print
	i_str cvs print ( ) print
	i_str cvs print (\n) print
} bind def


%    Copyright (C) 1994 Aladdin Enterprises.  All rights reserved.
% 
% This software is provided AS-IS with no warranty, either express or
% implied.
% 
% This software is distributed under license and may not be copied,
% modified or distributed except as expressly authorized under the terms
% of the license contained in the file LICENSE in this distribution.
% 
% For more information about licensing, please refer to
% http://www.ghostscript.com/licensing/. For information on
% commercial licensing, go to http://www.artifex.com/licensing/ or
% contact Artifex Software, Inc., 101 Lucas Valley Road #110,
% San Rafael, CA  94903, U.S.A., +1(415)492-9861.

% $Id$
% traceimg.ps
% Trace the data supplied to the 'image' operator.

% This code currently handles only the (Level 2) dictionary form of image,
% with a single data source and 8-bit pixels.

% changed for Scribus image import by Andreas Vox, 2006-2-21
% added support for colorimage and other image variant

/concatenate   % str1 str2 concatenate str12
{
	dup length 2 index length add string
	dup 3 index length 3 index putinterval
	dup 0 4 index putinterval
	exch pop exch pop
} bind def 

/==write  % any ==write -
{ 
	dup type dup /arraytype eq exch /packedarraytype eq or
	{ 
		i_file ([) writestring 
			{ ==write i_file ( ) writestring } forall 
		i_file (]) writestring
	} {
	dup type /nametype eq
	{
		i_file (/) writestring i_file exch i_str cvs writestring
	} {
	dup type /dicttype eq
	{
		i_file (<<) writestring
		{ ==write ( ) ==write ==write (\n) ==write } forall
		i_file (>>) writestring
	} {
		i_file exch i_str cvs writestring
	} ifelse } ifelse } ifelse
} def

/i_image			% <dict> i_image -
{
	begin 
		/i_left Width Height mul Decode length 2 idiv mul BitsPerComponent mul 8 idiv dup /i_size exch store store 
		/i_dict currentdict store 
		/i_nsources 1 store 
		/i_source 0 store 
		/i_datasource currentdict /DataSource get store
		currentdict /MultipleDataSources known not 
			{ /MultipleDataSources false def } if
		MultipleDataSources
		{
			/i_nsources  DataSource length store
			/i_datasource DataSource 0 get store
		} if
	end
	storeMatrix 
	i_dict /ImageMatrix get matrix invertmatrix matrix currentmatrix matrix concatmatrix /i_m exch def
	i_dict /Width get  0 i_m dtransform dup mul exch dup mul add sqrt /i_w exch def 
	0 i_dict /Height get i_m dtransform dup mul exch dup mul add sqrt /i_h exch def
	0  0 i_m transform  /i_y exch def /i_x exch def 
	/i_angle 0 def  % FIXME
	/ExportFiles where { /ExportFiles get (.) search { exch pop exch pop } if } { (imagefile) } ifelse
	    (-) concatenate dup /i_basename exch def i_filecount 9 string cvs concatenate
		{
			i_filecount 1 add /i_filecount exch store
			dup (.dat) concatenate status not { exit } if
			pop pop pop pop pop
			i_basename i_filecount 9 string cvs concatenate
		} loop
		(im ) print												% im x y w h angle ...
		i_x i_str cvs print ( ) print
		i_y i_str cvs print ( ) print
		i_w i_str cvs print ( ) print
		i_h i_str cvs print ( ) print
		i_angle i_str cvs print ( ) print
		i_dict /Width get  i_str cvs print ( ) print			% ... hpix vpix ...
		i_dict /Height get i_str cvs print ( ) print
		currentcolorspace 0 get /DeviceRGB eq
			{ (tiff24nc ) print } 
		{ currentcolorspace 0 get /DeviceCMYK eq
			{ (tiff32nc ) print } 
		{ currentcolorspace 0 get /DeviceGray eq
			{ (tiffgray ) print } 
			{ (tiff32nc ) print }
		ifelse } ifelse } ifelse
        dup  (.tif) concatenate print (\n) print flush        % ... dev filename
		(.dat) concatenate (w) file /i_file exch store
	currentcolorspace ==write ( setcolorspace\n) ==write
	(<<\n) ==write 
	i_dict { exch
		  dup /DataSource eq 
			{ pop pop (/DataSource currentfile\n) ==write }
		  {
			dup /ImageMatrix eq 
				{ pop pop (/ImageMatrix [1 0 0 -1 0 ) ==write i_dict /Height get ==write (]\n) ==write }
				{ ==write ( ) ==write ==write (\n) ==write }
			ifelse 
		  } ifelse 
		} forall 
	(>>\nimage\n) ==write i_file flushfile

    { %loop
      i_left 0 le 
      { 
		i_source 1 add /i_source exch def
        i_source i_nsources ge { exit } if
        i_dict /DataSource get i_source get /i_datasource exch def
		/i_left i_size def
      } if
      /i_datasource load exec
      dup type /filetype eq
       { i_buf 0 i_left 32 .min getinterval readstring pop
       } if
      dup i_file exch writestring 
      i_left exch length sub /i_left exch def
    } loop
    i_file flushfile
 } bind def

/colorimage
{
      % width height bits/sample matrix datasource0..n-1 multi ncomp
      /tmpN exch def
	  /tmpMulti exch def
	  tmpMulti
      {
         /tmpN load array astore
      } if
      /tmpN load 6 add dict
      dup 7 -1 roll /Width exch put 
      dup 6 -1 roll /Height exch put 
      dup 5 -1 roll /BitsPerComponent exch put 
      dup 4 -1 roll /ImageMatrix exch put 
      dup 3 -1 roll /DataSource exch put 
      tmpMulti
      {
         dup /MultipleDataSources true put
      } if
      dup /ImageType 1 put
      gsave
      /tmpN load
         dup 1 eq
         {
            1 index /Decode [0 1] /Decode put
            /DeviceGray setcolorspace
         } if
         dup 3 eq
         {
             1 index /Decode [0 1 0 1 0 1] put
            /DeviceRGB setcolorspace
         } if
         dup 4 eq
         {
            1 index /Decode [0 1 0 1 0 1 0 1]  put
            /DeviceCMYK setcolorspace
         } if
      pop
      i_image
	  grestore
} bind def

/image {
   gsave
   dup type /dicttype ne 
   {
      % width height bits/sample matrix datasource
      7 dict
      dup 7 -1 roll /Width exch put
      dup 6 -1 roll /Height exch put
      dup 5 -1 roll /BitsPerComponent  exch put
      dup 4 -1 roll /ImageMatrix exch put
      dup 3 -1 roll /DataSource exch put
      dup 1 /ImageType exch put
      dup [0 1] /Decode exch put
      /DeviceGray setcolorspace
   } if
   i_image
   grestore
} bind def

/imagemask
{
	dup type /dicttype ne 
	{
      % width height pol matrix datasource
      7 dict
      dup 7 -1 roll /Width exch put
      dup 6 -1 roll /Height exch put
      dup 5 -1 roll { [0 1] } { [1 0] } ifelse /Decode exch put
      dup 4 -1 roll /ImageMatrix exch put
      dup 3 -1 roll /DataSource exch put
	} if
    dup 1 /ImageType exch put
    dup 1 /BitsPerComponent exch put
	gsave
	/DeviceGray setcolorspace
	i_image
	grestore
	
	% for now only print the dict, FIXME
%	currentcolorspace 
%	dup == (setcolorspace) =
%	dup 0 get /DeviceGray eq
%		{ currentcolor == (setcolor) = }
%	{ dup 0 get /DeviceRGB eq
%		{ currentcolor == == == (setcolor) = }
%	{ dup 0 get /DeviceCMYK eq
%		{ currentcolor == == == == (setcolor) = }
%	{ currentcolor type /dicttype eq
%		{ (<<) = currentcolor { exch == == } forall (>>\nsetcolor) = }
%		{ currentcolor == (setcolor) = }
%	ifelse } ifelse } ifelse } ifelse pop
%	(<<) =
%	{ 
%		exch
%		== ==
%	} forall 
%	(>>\nimagemask) = flush
} bind def


% declare some global vars

/i_left 0 def
/i_size 0 def
/i_dict null def
/i_buf 32 string def
/i_nsources 1 def
/i_source 0 def
/i_datasource { (x) } def
/i_file null def
/i_filecount 1 def

%%%% End of traceimage code


/stateArray 500 array def
/stateTop 0 def
/gsave
{
	userdict begin
	(-gs\n) print
	stateArray stateTop gstate currentgstate put
	/stateTop stateTop 1 add def
	end
} bind def

/grestore
{
	userdict begin
	stateTop 1 lt
	{
	}
	{
		(-gr\n) print
		stateArray stateTop 1 sub get setgstate
		/stateTop stateTop 1 sub def
		stateArray stateTop 0 put
	}ifelse
	end
} bind def

% a bind def of the show operator doesn't work,
% so this is our way to get a charpath entry for flattening text
/root_charpath
{
	charpath
} bind def

/i_kerningI
{
	exch 1 getinterval stringwidth
} bind def

% find kerning value
/i_kerningII		% index string   i_kerning   dx dy 
{
	% stringwidth( [n..n+1] ) - stringwidth( [n+1] )
	/i_pstring exch def
	/i_pindex exch def
	i_pstring i_pindex 2 getinterval stringwidth exch	% y2 x2
	i_pstring i_pindex 1 add 1 getinterval stringwidth		% y2 x2 x1 y1
	4 1 roll sub											% y1 y2 (x2-x1)
	3 1 roll exch sub										% (x2-x1) (y2-y1)
} bind def


/i_kerningIII		% index string   i_kerning   dx dy 
{
	% stringwidth( [n..n+1] ) - stringwidth( [n+1] )
	/i_pstring exch def
	/i_pindex exch def
	i_pstring i_pindex 2 getinterval (l) exch concatenate stringwidth exch	% y2 x2
	i_pstring i_pindex 1 add 1 getinterval (l) exch concatenate stringwidth		% y2 x2 x1 y1
	4 1 roll sub											% y1 y2 (x2-x1)
	3 1 roll exch sub										% (x2-x1) (y2-y1)
} bind def

/i_kerning /i_kerningII load def

/show % string show -
{ 
	userdict begin
	storeMatrix
	currentfont /FontName known
	% stack: string
	{
		currentpoint /ycur exch def /xcur exch def
		currentpoint	% x y
		newpath
		/clipCnt 0 def
		moveto
		/completeString exch def
		% we process each char separately to get smaller paths
		0 1 completeString length 1 sub
		{
			(n\n)print			% start polygon
			writecurrentcolor	% write color
			storeMatrix
			dup completeString length 1 sub eq 
			{ 0 0 } { dup completeString i_kerning } ifelse 
			/curwidthy exch def /curwidthx exch def
			completeString exch 1 getinterval dup /curstr exch def
			false root_charpath
			{_move} {_line} {_curve} {_close} pathforall
			(f\n)print			% close polygon
			newpath
			curwidthx xcur add curwidthy ycur add moveto
			currentpoint /ycur exch def /xcur exch def
			newpath			% clear graphic stack
			xcur ycur moveto
		} for
		currentpoint	% x y
		newpath				% clear graphic stack (and current point)
		moveto
	} {
		pop		% string
	} ifelse
	end
} bind def

/ashow
{
	% ax ay string
	exch /ydist exch def
	exch /xdist exch def
	userdict begin
	storeMatrix
	currentfont /FontName known
	% stack: string
	{
		currentpoint /ycur exch def /xcur exch def
		currentpoint	% x y
		newpath
		/clipCnt 0 def
		moveto
		/completeString exch def
		% we process each char separately to get smaller paths
		0 1 completeString length 1 sub
		{
			(n\n)print			% start polygon
			writecurrentcolor	% write color
			storeMatrix
			dup completeString length 1 sub eq 
			{ 0 0 } { dup completeString i_kerning } ifelse 
			/curwidthy exch def /curwidthx exch def
			completeString exch 1 getinterval dup /curstr exch def 
			false root_charpath
			{_move} {_line} {_curve} {_close} pathforall
			(f\n)print			% close polygon
			newpath
			curwidthx xcur add curwidthy ycur add
			exch xdist add exch ydist add moveto
			currentpoint /ycur exch def /xcur exch def
			newpath			% clear graphic stack
			xcur ycur moveto
		} for
		currentpoint	% x y
		newpath				% clear graphic stack (and current point)
		moveto
	} {
		pop
	} ifelse
	end
} bind def

/awidthshow		% cx cy char ax ay string
{
	% ax ay string
	exch /ydist exch def
	exch /xdist exch def
	% cx cy char string
	exch /char exch def
	exch /cydist exch def
	exch /cxdist exch def
	userdict begin
	storeMatrix
	currentfont /FontName known
	% stack: string
	{
		currentpoint /ycur exch def /xcur exch def
		currentpoint	% x y
		newpath
		/clipCnt 0 def
		moveto
		/completeString exch def
		% we process each char separately to get smaller paths
		0 1 completeString length 1 sub
		{
			(n\n)print			% start polygon
			writecurrentcolor	% write color
			storeMatrix
			dup completeString length 1 sub eq 
			{ 0 0 } { dup completeString i_kerning } ifelse 
			/curwidthy exch def /curwidthx exch def
			completeString exch 1 getinterval dup /curstr exch def 
			false root_charpath
			{_move} {_line} {_curve} {_close} pathforall
			(f\n)print			% close polygon
			newpath
			curwidthx xcur add curwidthy ycur add
			exch xdist add exch ydist add moveto
			curstr 0 get char eq
			{
				currentpoint exch cxdist add exch cydist add moveto
			} if
			currentpoint /ycur exch def /xcur exch def
			newpath			% clear graphic stack
			xcur ycur moveto
		} for
		currentpoint	% x y
		newpath				% clear graphic stack (and current point)
		moveto
	} {
		pop
	} ifelse
	end
} bind def

/widthshow	% cx cy char string
{
	0 exch
	0 exch
	awidthshow
} bind def

/cshow	% proc string
{
	exch pop
	show
} bind def

/kshow	% proc string
{
	dup length 1 sub 
	dup 0 ne
	{
		1 index 0 1 getinterval
		show
		1 sub
		dup 0 ne
		{
			1 add
			1 exch 1 exch
			{
				dup 1 sub
				2 index exch get
				2 index 2 index get
				4 index exec
				1 index exch 1 getinterval
				show
			} for
		} if
	}
	{
		pop dup show
	} ifelse
	pop pop
} bind def

/xshow	% string array
{
	pop
	show
} bind def

/xyshow	% string array
{
	pop
	show
} bind def

/yshow	% string array
{
	pop
	show
} bind def

/ReEnc % newfontname reencodevector origfontdict -> ReEncode -> newfontdict
{
	userdict begin
 dup begin dup maxlength dict begin
  { 1 index /FID ne {def} {pop pop} ifelse
  } forall
  /Encoding exch def
  currentdict
  end end
  definefont
	end
} bind def

/glyphshow {
    save % So can reclaim VM from reencoding
    currentfont /Encoding get dup length array copy dup 0 5 -1 roll put
    /GlyphShowTempFont exch currentfont ReEnc
    setfont
    (\000) show 
	restore
} bind def

/showpage
{
	(sp\n) print
} bind def

