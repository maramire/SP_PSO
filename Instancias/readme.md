---------------------------------------------------------------------------------
Data Source		Set	Instances	n		w	h	|
---------------------------------------------------------------------------------
Hopper & Turton (2001)	C	21		16-197		20-160	15-240	|
Hopper (2000)		N	35		17-199		200	200	|
			T	35		17-199		200	200	|
Babu & Babu (1999)	Babu	1		50		1000	375	|
Burke et al. (2004)	Burke	13		10-3152		30-640	40-960	|
Pinto & Oliveira (2005)	CX	7		50-15000	400	600	|
Wang & Valenzela (2001)	Nice	6		25-1000		100	100	|
			Path	6		25-1000		100	100	|
-
Beasley (1985b)		ngcut	12		7-22		10-30	-	|
Beasley (1985a)		gcut	13		10-50		250-3000-	|
Christofides (1977)	cgcut	3		16-60		10-70	-	|
Bengtsson (1982)	beng	10		20-200		25-40	-	|
Berkey & Wang (1987)	bwmv	300		20-100		10-100	-	|
Martello & Vigo (1998)	bwmv	200		20-100		10-100	-	|
Bortfeldt (2006)	AH	360		1000		1000	-	|
---------------------------------------------------------------------------------



--------------------------------------------------------------------------------------------------------------------------------------------
File-structure: each changed data file is described as follows:

(Data for strip)
Line 01:      W LB                            :size of the strip
                                                      W:Strip-width
                                                     LB:Strip-height(lower bound)
Line 02:      n                                    :Number of items

(Data for items)
Line 02 + 1:      w h                           : data for item 1
                                                        w: width of item 1
                                                         h: height of item 1
.....................
Line 02+i                                          :data for item  i
.....................
Line 02+n                                         :data for item  n
-------------------------------------------------------------------------------------------------------
