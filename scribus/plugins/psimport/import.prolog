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
% pat # #           makepattern params tmpfilename
% mask				imagemask, followed by b/w image
% fill-evenodd		set fill rule
% fill-winding		- " -
% 15.05.2004 Added the Glyphshow Operator.
% 17.05.2004 Made clipping working.
% 20.05.2004 kshow is working now.
% 22.02.2006 added image and colorimage ops -- av
% 02.03.2006 added code to divide reported coordinates by (device resolution/72) -- av
% 23.03.2006 added code to trace PDF commands

% some hacks to get access to PDF operators, needs -sDELAYBIND

currentglobal true setglobal
GS_PDF_ProcSet begin
currentdict /setshowstate
{
	dup WordSpacing 0 32 TextSpacing 0 6 -1 roll awidthshow
	//setshowstate exec
} .forceput 
currentdict /endpage {
	(sp\n) print
	//endpage exec
} .forceput
currentdict /doimage known not {
	currentdict /doimage { } .forceput
} if
currentdict /doimage {
	currentdict i_image
	//doimage exec
} .forceput
end
setglobal
.bindnow

/cfile TraceFile (w) file def
/print { cfile exch writestring } bind def


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
	dup type /stringtype eq
	{
		true 1 index { 32 ge and } forall
		{
			i_file (\() writestring i_file exch writestring i_file (\)) writestring
		}
		{
			i_file (<) writestring
			i_file /ASCIIHexEncode filter dup
			3 -1 roll
			writestring closefile % close filter 
			i_file (\n) writestring			
		} ifelse
	} {
	dup type /dicttype eq
	{
		i_file (<<) writestring
		{ ==write ( ) =write ==write (\n) =write } forall
		i_file (>>) writestring
	} {
		i_file exch i_str cvs
		dup dup length 1 sub get (-) 0 get eq { pop (null) } if 
		writestring
	} ifelse } ifelse } ifelse } ifelse
} def

/=write  % any =write -
{ 
	dup type dup /arraytype eq exch /packedarraytype eq or
	{ 
			{ =write i_file ( ) writestring } forall 
	} {
	dup type /nametype eq
	{
		i_file exch i_str cvs writestring
	} {
	dup type /stringtype eq
	{
		i_file exch writestring
	} {
	dup type /dicttype eq
	{
		i_file (<<) writestring
		{ =write ( ) =write =write (\n) =write } forall
		i_file (>>) writestring
	} {
		i_file exch i_str cvs writestring
	} ifelse } ifelse } ifelse } ifelse
} def


% flag to deactivate our substitutions
/i_shortcut false def

% defines an overloaded function     name proc i_shortcutOverload -
% equvalent to /name { i_shortcut { //name } { proc } ifelse } bind def
/i_shortcutOverload
{
	[ /i_shortcut /load load [ 5 index load ] cvx 4 index /ifelse load ] cvx 
	exch pop 
	bind def
} def

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

currentpagedevice /HWResolution get aload pop
72 div /i_vscale exch def
72 div /i_hscale exch def

/m_a 1 def
/m_b 0 def
/m_c 0 def
/m_d 1 def
/m_x 0 def
/m_y 0 def
/matrix_x	% x y
{
	% ax + cy + tx
	m_c mul exch m_a mul add m_x add i_hscale div
} bind def
/matrix_y	% x y
{
	% bx + dy + ty
	m_d mul exch m_b mul add m_y add i_vscale div
} bind def


/concatenate   % str1 str2 concatenate str12
{
	dup length 2 index length add string
	dup 3 index length 3 index putinterval
	dup 0 4 index putinterval
	exch pop exch pop
} bind def 

% this is like search but returns the last match in string
/rsearch	% string seek  rsearch  post match pre true // string false
{
	2 copy search	% string seek post1 match1 pre1 true
	{
		2 index 4 index rsearch		% string seek post1 match1 pre1 post2 match2 pre2 true
		{
			6 -1 roll pop			% string seek match1 pre1 post2 match2 pre2 
			% combine (pre1 match1 pre2) into one string
			5 -1 roll exch concatenate	% string seek pre1 post2 match2 (match1+pre2)
			4 -1 roll exch concatenate	% string seek post2 match2 (pre1+match1+pre2)
		} {		% string seek post1 match1 pre1 post1
			pop
		} ifelse
		% string seek post match pre
		5 -2 roll pop pop
		true
	} {		% string seek string 
		pop pop false
	} ifelse
} bind def


% returns a unique filename for the given extension
/i_exportfilename   % string i_exportfilename string
{
	/ExportFiles where { /ExportFiles get (.) rsearch { exch pop exch pop } if } { (imagefile) } ifelse
	    (-) concatenate dup /i_basename exch def i_filecount 9 string cvs concatenate
		{
			i_filecount 1 add /i_filecount exch store
			dup 2 index concatenate status not { exit } if
			pop pop pop pop pop
			i_basename i_filecount 9 string cvs concatenate
		} loop
	exch pop
} bind def

% Code for reading patters is currently commented out, as it
% doesn't seem to work correctly.
% /makepattern { % dict matrix  makepattern patterndict
% %/makepattern =
% 	% we will do some real painting here:
% 	/i_shortcut true store
% 	% params:
% 	/i_m exch def
% 	/i_dict exch def
% 	% define export filename	
% 	/i_basename (.png) i_exportfilename (.png) concatenate def
% 	i_dict /BBox get
% 		dup 0 get /i_x exch def
% 		dup 1 get /i_y exch def
% 		dup 2 get i_x sub /i_w exch def
% 		3 get i_y sub /i_h exch def
% 	% we want those in devspace:
% 		i_x i_y i_m itransform matrix currentmatrix transform
% 			i_vscale div /i_y exch def i_hscale div /i_x exch def
% 		i_w i_h i_m idtransform matrix currentmatrix dtransform
% 			i_vscale div /i_h exch def i_hscale div /i_w exch def
% 		% i_h < 0 ?
% 		i_h 0 le
% 		{
% 			/i_y i_h i_y add def
% 			/i_h i_h neg def
% 		} if
% 		% i_w < 0 ?
% 		i_w 0 le
% 		{
% 			/i_x i_w i_x add def
% 			/i_w i_w neg def
% 		} if
% 	% now we can use the current matrix as pattern matrix, but with (0,0) origin
% 	i_m ==
% 	i_x i_y matrix currentmatrix translate /i_m exch def
% 	i_m ==
% 	i_dict /BBox [ 0 0 i_w i_h ] put 
% 	(w x h =) = i_w = i_h =
% 	% paint pattern to png file
% 	gsave
% 	currentcolor currentcolorspace
% 	<< 
% 		/OutputFile i_basename
% 		/OutputDevice (pngalpha)
% 		/TextAlphaBits 4
% 		/GraphicsAlphaBits 4
% %		/BackgroundColor 16777215
% %		/BackgroundColor 0
% 		/PageUsesTransparency true
% 		/HWResolution [ 72 72 ]
% 		/ProcessColorModel /DeviceRGB
% 		/PageSize [i_w i_h]
% 	/pngalpha finddevice putdeviceprops setdevice 
% 	setcolorspace setcolor
% %	matrix currentmatrix ==
% %	0 0 transform exch = =
% %	1 1 transform exch = =
% 	i_dict i_w i_h matrix identmatrix scale 
% 			%matrix identmatrix 
% 			//makepattern setpattern
% 	0 0 i_w i_h rectfill
% 	showpage
% 	grestore
% 	% create pattern with our extensions:
% 	i_dict dup /ExportFile i_basename put
% 	dup /Origin [ 0 0 transform ] put
% 	i_m //makepattern
% 	/i_shortcut false store
% %/makepatternE =
% } i_shortcutOverload

/writecurrentpattern
{
	currentcolor
	(pat ) print
	dup /Origin get
	dup 0 get i_hscale div i_str cvs print ( ) print
	    1 get i_vscale div i_str cvs print ( ) print
	/ExportFile get print
	(\n) print
} bind def

/writecurrentcmykcolor
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
} bind def


/writecurrentrgbcolor
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
} bind def


/writecurrentcolor
{
	currentcolorspace 0 get 

	% try to find a base colorspace first
	dup	/Indexed eq
	{
		pop
		currentcolorspace 1 get
		dup type /arraytype eq { 0 get } if
	} if

	dup dup /DeviceN eq exch /Separation eq or
	{
		pop
		currentcolorspace 2 get
		dup type /arraytype eq { 0 get } if
	} if

	% now write values
	dup /CIEBasedABC eq
	{ % this must be a hack....
		gsave
		currentcolor setrgbcolor
		writecurrentrgbcolor
		grestore
	} {
 	dup /DeviceRGB eq
	{
		writecurrentrgbcolor
	} {
	dup dup /DeviceCMYK eq exch /DeviceGray eq or
	{
		writecurrentcmykcolor
	} { 
	dup /Pattern eq
	{
		writecurrentpattern
	} {
		% TODO: other CIE
		writecurrentrgbcolor % will always be 0
	}
	ifelse } ifelse } ifelse } ifelse
	pop
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
		1 index exch get
		storeMatrix
		dup dup dup m_b abs mul exch m_d abs mul add  exch m_a abs mul add  exch m_c abs mul add  2 div  abs
		i_hscale div
		i_str cvs print ( ) print
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
%??	
	dup dup dup m_b abs mul exch m_d abs mul add  exch m_a abs mul add  exch m_c abs mul add  2 div  abs
	i_hscale div
	% transform (w,w) and take length
%av-test:	dup dtransform i_vscale div dup mul exch i_hscale div dup mul add sqrt
	(w ) print
	i_str cvs print
	(\n) print
	end
} bind def

/i_move	% x y
{
	userdict begin
	(m\n) print
	/currentY exch def
	/currentX exch def
	/beginX currentX def
	/beginY currentY def
	end
} bind def

/i_line
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

/i_curve
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
/i_close
{
	beginX beginY i_line
	(cp\n) print
} bind def

/storeMatrix
{
	userdict begin
	matrix currentmatrix
	dup 0 get /m_a exch def
	dup 1 get /m_b exch def
	dup 2 get /m_c exch def
	dup 3 get /m_d exch def
	dup 4 get /m_x exch def
	5 get /m_y exch def
	end
} bind def

/pathClipAndClose % this is not nice: closes all open paths & flattens the path :-(
{
	clipsave 
	clip				% combine clippath and path
	newpath clippath	% copy (closed) clippath to path
	cliprestore
} bind def


% find out if the point is within the clipping area
/i_in_clip					% x y  i_in_clip  bool
{
	gsave
	newpath clippath
	infill
	grestore
} bind def


% find out if two points are within the clipping area
/i_in_clip2					% x1 y1 x2 y2 i_in_clip  bool1 bool2
{
	gsave
	newpath clippath
	infill					% x1 y1 bool2
	3 1 roll				% bool2 x1 y1
	infill					% bool2 bool1
	exch
	grestore
} bind def


/i_clip_move
{
	/beginY exch store 
	/beginX exch store 
	/currentX beginX store
	/currentY beginY store
	% test if within cliparea
	currentX currentY i_in_clip
	{
		currentX currentY /moveto load
	} if
} bind def


% find intersection with line x1,y1 -> x2,y2 with clip path.
% x1,y2 is outside the clip area, x2, y2, x3, y3 inside

/i_find_clip_intersect		% x1 y1 x2 y2  i_find_clip_intersect  x3 y3
{
	3 index 2 index sub		% x1 y1 x2 y2 dx
	3 index 2 index sub		% x1 y1 x2 y2 dx dy
	gsave
	newpath clippath
	{
		2 div exch 2 div exch				% half interval
		2 copy abs 0.01 lt exch abs 0.01 lt and
			{ exit } if						% done
		2 copy 4 index add exch				% x1 y1 x2 y2 dx dy (y2+dy) dx
		       5 index add exch				% x1 y1 x2 y2 dx dy (x2+dx) (y2+dy)
%		/Intersect = 7 index = 6 index = 5 index = 4 index = 3 index = 2 index = 1 index = 0 index = 
		2 copy infill
		{									% replace x2,y2
			6 -2 roll pop pop
			4 2 roll
		} {
			8 -2 roll pop pop				% replace x1,y1
			6 2 roll
		} ifelse
	} loop
	grestore
	6 -2 roll								% return x2,y2
	4 { pop } repeat
} bind def

/i_clip_line
{
	/endY exch store
	/endX exch store
	currentX currentY endX endY i_in_clip2 
	{ % end in
		{  
			% both in. just draw it. FIXME check if line leaves cliparea
			endX endY /lineto load
		} {
			% current not in
			% find new current point
			currentX currentY endX endY i_find_clip_intersect 
			/moveto load
			endX endY /lineto load
		} ifelse
	} { % end not in
		{   % current in
			% find new endpoint
			endX endY currentX currentY i_find_clip_intersect 
			/lineto load
		} {
			% both not in
			% try to find a point within cliparea
			currentX currentY endX endY i_find_clip_intersect
			2 copy i_in_clip
			{
				% yeah
				/moveto load
				% now find point from other end
				endX endY currentX currentY i_find_clip_intersect
				/lineto load
			} {
				pop pop
			} ifelse
		} ifelse
	} ifelse
	/currentX endX store
	/currentY endY store
} bind def


/pathClipForStroke 
{
	% only lines
	flattenpath
	% create a userpath from currentpath
	userdict begin
	/beginX 0 def /beginY 0 def
	/currentX 0 def /currentY 0 def
	/endX 0 def /endY 0 def
	systemdict begin							% some EPS redefine moveto & Co :-(
	[
			{ i_clip_move }						% remember last move
			{ i_clip_line }						% clip lines individually
			{ 6 {pop} repeat /OOPS = }			% won't happen
			{ beginX beginY i_clip_line }		% close with line
		pathforall
	] cvx 
%	dup ==
	newpath % uappend % userpaths SUCK!
	end end
	exec
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
		xr yr i_move
		xr wr add yr i_line
		xr wr add yr hr add i_line
		xr yr hr add i_line
		xr yr i_line
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
			xr yr i_move
			xr wr add yr i_line
			xr wr add yr hr add i_line
			xr yr hr add i_line
			xr yr i_line
		} for
	}ifelse
	(cp\n)print
	(f\n)print			% close polygon
	end
} i_shortcutOverload

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
	dup type dup /arraytype ne exch /stringtype ne and
	{
		/hr exch def
		/wr exch def
		/yr exch def
		/xr exch def
		xr yr i_move
		xr wr add yr i_line
		xr wr add yr hr add i_line
		xr yr hr add i_line
		xr yr i_line
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
			xr yr i_move
			xr wr add yr i_line
			xr wr add yr hr add i_line
			xr yr hr add i_line
			xr yr i_line
		} for
	}ifelse
	(cp\n)print
	(s\n)print			% stroke rect
	end
} i_shortcutOverload

/stroke
{
	(n\n) print
	writecurrentcolor
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
%	clipCnt 1 eq 
%		{ pathClipForStroke } if
	storeMatrix

	{i_move} {i_line} {i_curve} {i_close} pathforall
	(s\n)print			% stroke path
	newpath
} i_shortcutOverload

/eofill
{
	(n\n) print			% start polygon
	writecurrentcolor	% write color
	writecurrentlinewidth
	writecurrentlinecap
	writecurrentlinejoin
	writecurrentdash
%	clipCnt 1 eq
%		{ pathClipAndClose } if
	storeMatrix			% take transformation, scaling, rotation from PostScript
	{i_move} {i_line} {i_curve} {i_close} pathforall
	(f\n)print			% close polygon

	newpath				% clear stack
} i_shortcutOverload

/fill 
{
	(fill-winding\n) print
	eofill 
	(fill-evenodd\n) print
} i_shortcutOverload

/clip
{
	userdict begin
	(n\n)print			% start clip polygon

% FIXME: pathClipAndClose first?

	storeMatrix			% take transformation, scaling, rotation from PostScript
	{i_move} {i_line} {i_curve} {i_close} pathforall

	(ci\n)print			% close clip polygon begin path
						% we have to close the path!!
%	clip
%	/clipCnt 1 def
	newpath				% clear stack
	end
} i_shortcutOverload

/eoclip
{
	userdict begin
	(n\n)print			% start clip polygon

% FIXME: pathClipAndClose first?

	storeMatrix			% take transformation, scaling, rotation from PostScript
	{i_move} {i_line} {i_curve} {i_close} pathforall

	(ci\n)print			% close clip polygon begin path
						% we have to close the path!!
%	clip
%	/clipCnt 1 def
	newpath				% clear stack
	end
} i_shortcutOverload

% we don't clip
% because this doesn't work for flattening text (show, charpath) with NeXT PostScript Code
/rectclip
{
	% let Scribus decide what to do with ci; was: pop pop pop pop
	userdict begin
	(n\n)print			% start clip polygon

	storeMatrix			% take transformation, scaling, rotation from PostScript
	dup type dup /arraytype ne exch /stringtype ne and
	{
		4 copy
		/i_h exch def
		/i_w exch def
		/i_y exch def
		/i_x exch def
		i_x i_y i_move
		i_x i_w add i_y i_line
		i_x i_w add i_y i_h add i_line
		i_x i_y i_h add i_line
	} {
		% array or string
		0 4 dup length 1 sub 
		{
			1 index 1 index get /i_x exch def
			1 add
			1 index 1 index get /i_y exch def
			1 add
			1 index 1 index get /i_w exch def
			1 add
			1 index 1 index get /i_y exch def
			i_x i_y i_move
			i_x i_w add i_y i_line
			i_x i_w add i_y i_h add i_line
			i_x i_y i_h add i_line
		} for
	} ifelse
	
	(ci\n)print			% close clip polygon begin path
						% we have to close the path!!
%	rectclip
%	/clipCnt 1 def
	newpath				% clear stack
	end
} i_shortcutOverload


% Code for reading images is currently commented out, as it
% doesn't seem to work correctly.
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

% /i_image			% <dict> i_image -
% {
% %dup { == == } forall
% /i_image =
% 	begin 
% 		/i_left Width Height mul Decode length 2 idiv mul BitsPerComponent mul 8 idiv dup /i_size exch store store 
% 		/i_dict currentdict store 
% 		/i_nsources 1 store 
% 		/i_source 0 store 
% 		/i_datasource currentdict /DataSource get store
% 		currentdict /MultipleDataSources known not 
% 			{ /MultipleDataSources false def } if
% 		MultipleDataSources
% 		{
% 			/i_nsources  DataSource length store
% 			/i_datasource DataSource 0 get store
% 		} if
% 	end
% 	storeMatrix 
% 	i_dict /ImageMatrix get matrix invertmatrix matrix currentmatrix matrix concatmatrix /i_m exch def
% 	i_dict /Width get  0 i_m dtransform dup mul exch dup mul add sqrt /i_w exch def 
% 	0 i_dict /Height get i_m dtransform dup mul exch dup mul add sqrt /i_h exch def
% 	0  0 i_m transform  /i_y exch def /i_x exch def 
% 	i_dict /Width get i_dict /Height get i_m transform
% 	/i_hflip -1 def /i_vflip 1 def 
% 	dup i_y le { /i_y exch def } { pop /i_vflip -1 def } ifelse
% 	dup i_x le { /i_x exch def } { pop /i_hflip  1 def } ifelse
% 	0 i_dict /Height get i_m dtransform atan
% 	/i_angle exch def
% 	(.dat) i_exportfilename
% 		(im ) print			% im x y w h angle ...
% 		i_x i_hscale div i_str cvs print ( ) print
% 		i_y i_vscale div i_str cvs print ( ) print
% 		i_w i_hscale div i_str cvs print ( ) print
% 		i_h i_vscale div i_str cvs print ( ) print
% 		i_angle i_str cvs print ( ) print
% 		i_dict /Width get  i_str cvs print ( ) print			% ... hpix vpix ...
% 		i_dict /Height get i_str cvs print ( ) print
% 		currentcolorspace 0 get /DeviceRGB eq
% 			{ (tiff24nc ) print } 
% 		{ currentcolorspace 0 get /DeviceCMYK eq
% 			{ (psdcmyk ) print } 
% 		{ currentcolorspace 0 get /DeviceGray eq
% 			{ (tiffgray ) print } 
% 			{ (tiff32nc ) print }
% 		ifelse } ifelse } ifelse
%         dup  (.tif) concatenate print (\n) print flush			% ... dev filename
% 		(.dat) concatenate (w) file /i_file exch store			% temp file
% 	currentcolorspace ==write ( setcolorspace\n) =write
% 	(<<\n) =write 
% 	i_dict { exch
% 		  dup /DataSource eq 
% 			{ pop pop (/DataSource currentfile\n) =write }
% 		  {
% 			dup /ImageMatrix eq 
% 				{ pop pop (/ImageMatrix [) =write
% 					i_hflip ==write ( 0 0 ) =write i_vflip ==write 
% 					( ) =write 
% 					i_hflip 0 lt { i_dict /Width get } { 0 } ifelse ==write 
% 					( ) =write 
% 					i_vflip 0 lt { i_dict /Height get} { 0 } ifelse ==write 
% 					(]\n) =write }
% 				{ ==write ( ) =write ==write (\n) =write }
% 			ifelse 
% 		  } ifelse 
% 		} forall 
% 	(>>\nimage\n) =write i_file flushfile
% 
%     { %loop
%       i_left 0 le 
%       { 
% 		i_source 1 add /i_source exch def
%         i_source i_nsources ge { exit } if
%         i_dict /DataSource get i_source get /i_datasource exch def
% 		/i_left i_size def
%       } if
%       /i_datasource load exec
%       dup type /filetype eq
%        { i_buf 0 i_left 32 .min getinterval readstring pop
%        } if
%       dup i_file exch writestring 
%       i_left exch length sub /i_left exch def
%     } loop
%     i_file flushfile
% /i_imageE =
%  } bind def
% 
% /colorimage
% {
% /colorimage =
% 	% width height bits/sample matrix datasource0..n-1 multi ncomp
% 	/tmpN exch def
% 	/tmpMulti exch def
% 	tmpMulti
% 	{
% 		/tmpN load array astore
% 	} if
% 	/tmpN load 6 add dict
% 	dup 7 -1 roll /Width exch put 
% 	dup 6 -1 roll /Height exch put 
% 	dup 5 -1 roll /BitsPerComponent exch put 
% 	dup 4 -1 roll /ImageMatrix exch put 
% 	dup 3 -1 roll /DataSource exch put 
% 	tmpMulti
% 	{
% 		dup /MultipleDataSources true put
% 	} if
% 	dup /ImageType 1 put
% 	gsave
% 	/tmpN load
% 		dup 1 eq
% 		{
% 			1 index /Decode [0 1] /Decode put
%             /DeviceGray setcolorspace
% 		} if
% 		dup 3 eq
% 		{
% 			1 index /Decode [0 1 0 1 0 1] put
% 			/DeviceRGB setcolorspace
% 		} if
% 		dup 4 eq
% 		{
% 			1 index /Decode [0 1 0 1 0 1 0 1]  put
% 			/DeviceCMYK setcolorspace
% 		} if
% 	pop
% 	i_image
% 	grestore
% /colorimageE =
% } i_shortcutOverload
% 
% /image {
% /image =
% 	gsave
% 	dup type /dicttype ne 
% 	{
% 		% width height bits/sample matrix datasource
% 		7 dict
% 		dup 7 -1 roll /Width exch put
% 		dup 6 -1 roll /Height exch put
% 		dup 5 -1 roll /BitsPerComponent  exch put
% 		dup 4 -1 roll /ImageMatrix exch put
% 		dup 3 -1 roll /DataSource exch put
% 		dup 1 /ImageType exch put
% 		dup [0 1] /Decode exch put
% 		/DeviceGray setcolorspace
% 	} if
% 	i_image
% 	grestore
% /imageE =
% } i_shortcutOverload
% 
% /imagemask
% {
% /imagemask =
% 	writecurrentcolor
% 	(mask\n) print
% 	gsave
% 	dup type /dicttype ne 
% 	{
% 		% width height pol matrix datasource
% 		7 dict
% 		dup 7 -1 roll /Width exch put
% 		dup 6 -1 roll /Height exch put
% 		dup 5 -1 roll { [0 1] } { [1 0] } ifelse /Decode exch put
% 		dup 4 -1 roll /ImageMatrix exch put
% 		dup 3 -1 roll /DataSource exch put
% 	} if
% 	dup 1 /ImageType exch put
% 	dup 1 /BitsPerComponent exch put
% 	/DeviceGray setcolorspace
% 	i_image
% 	grestore
% /imagemaskE =
% } i_shortcutOverload
% 
% 
% % declare some global vars
% 
% /i_left 0 def
% /i_size 0 def
% /i_dict null def
% /i_buf 32 string def
% /i_nsources 1 def
% /i_source 0 def
% /i_datasource { (x) } def
% /i_file null def
% /i_filecount 1 def
% 
%%%% End of traceimage code


/stateArray 500 array def
/stateTop 0 def
/gsave
{
	(gs\n) print
	userdict begin
%	(gs\n) print
	stateArray stateTop gstate currentgstate put
	/stateTop stateTop 1 add def
	end
} i_shortcutOverload

/grestore
{
	(gr\n) print
	userdict begin
	stateTop 1 lt
	{
	}
	{
%		(gr\n) print
		stateArray stateTop 1 sub get setgstate
		/stateTop stateTop 1 sub def
		stateArray stateTop 0 put
	}ifelse
	end
} i_shortcutOverload

/stringwidth
{
	/i_shortcut true store
	stringwidth
	/i_shortcut false store
} i_shortcutOverload

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
		currentfont /FontType get dup 3 eq exch 0 eq or
		{
			currentpoint /ycur exch def /xcur exch def
			currentpoint	% x y
			newpath
			/clipCnt 0 def
			moveto
			(n\n)print			% start polygon
			writecurrentcolor	% write color
			storeMatrix
			dup
			stringwidth
			/curwidthy exch def /curwidthx exch def
			false root_charpath
			{i_move} {i_line} {i_curve} {i_close} pathforall
			(f\n)print			% close polygon
			newpath
			curwidthx xcur add curwidthy ycur add moveto
			currentpoint /ycur exch def /xcur exch def
			newpath			% clear graphic stack
			xcur ycur moveto
		}
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
				{ dup completeString exch 1 getinterval stringwidth } 
				{ dup completeString i_kerning } ifelse 
				/curwidthy exch def /curwidthx exch def
				completeString exch 1 getinterval dup /curstr exch def
				false root_charpath
				{i_move} {i_line} {i_curve} {i_close} pathforall
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
		} ifelse
	} 
	{
		currentfont /FontType known
		{
			currentfont /FontType get dup 3 eq exch 0 eq or
			{
				currentpoint /ycur exch def /xcur exch def
				currentpoint	% x y
				newpath
				/clipCnt 0 def
				moveto
				(n\n)print			% start polygon
				writecurrentcolor	% write color
				storeMatrix
				dup
				stringwidth
				/curwidthy exch def /curwidthx exch def
				false root_charpath
				{i_move} {i_line} {i_curve} {i_close} pathforall
				(f\n)print			% close polygon
				newpath
				curwidthx xcur add curwidthy ycur add moveto
				currentpoint /ycur exch def /xcur exch def
				newpath			% clear graphic stack
				xcur ycur moveto
			}
			{
				pop
			} ifelse
		} 
		{ 
			pop
		} ifelse
	} ifelse
	end
} i_shortcutOverload

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
			{ dup completeString exch 1 getinterval stringwidth } 
			{ dup completeString i_kerning } ifelse 
			/curwidthy exch def /curwidthx exch def
			completeString exch 1 getinterval dup /curstr exch def
			false root_charpath
			{i_move} {i_line} {i_curve} {i_close} pathforall
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
} i_shortcutOverload

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
			{ dup completeString exch 1 getinterval stringwidth } 
			{ dup completeString i_kerning } ifelse 
			/curwidthy exch def /curwidthx exch def
			completeString exch 1 getinterval dup /curstr exch def
			false root_charpath
			{i_move} {i_line} {i_curve} {i_close} pathforall
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
} i_shortcutOverload

/widthshow	% cx cy char string
{
	0 exch
	0 exch
	awidthshow
} bind def

%/cshow	% proc string
%{
%	exch pop
%	show
%} i_shortcutOverload

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
} i_shortcutOverload

/xshow	% string array
{
	pop %FIXME
	show
} i_shortcutOverload

/xyshow	% string array
{
	pop %FIXME
	show
} i_shortcutOverload

/yshow	% string array
{
	pop %FIXME
	show
} i_shortcutOverload

/i_reencode % newfontname reencodevector origfontdict -> i_reencode -> newfontdict
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
    /GlyphShowTempFont exch currentfont i_reencode
    setfont
    (\000) show 
	currentpoint 3 -1 roll % curx cury -save-
	restore
	newpath
	moveto
} i_shortcutOverload

/showpage
{
	(sp\n) print
} i_shortcutOverload

