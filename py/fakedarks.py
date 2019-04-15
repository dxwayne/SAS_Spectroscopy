#!/usr/bin/env python3
# -*- coding: latin-1 -*-
# HEREHEREHERE

#############################################################################
# 
#  /home/wayne/git/SAS_Spectroscopy/py/__pycache__
#
#emacs helpers
# (insert (buffer-file-name))
#
# (ediff-current-file)
# (wg-python-fix-pdbrc)
# (find-file-other-frame "./.pdbrc")
# (wg-python-fix-pdbrc)   # PDB DASH DEBUG end-comments
#
# (setq mypdbcmd (concat (buffer-file-name) "<args...>"))
# (progn (wg-python-fix-pdbrc) (pdb mypdbcmd))
#
# (wg-astroconda-pdb)       # IRAF27
# (wg-astroconda3-pdb)      # CONDA Python3
#
# (set-background-color "light blue")
# (wg-python-toc)
#               
#############################################################################
import optparse
import os
import re
import sys
import datetime

try:
    import numpy as np
    from astropy.io import fits
except:
    print('Make sure that anaconda is installed.')
    sys.exit(-1)

__doc__ = """

/home/wayne/git/SAS_Spectroscopy/py/fakedarks.py options
[options]

This produces a file called fakedark.fits
The options are hardwired for an ATIK 414EX camera.

They include:
-x, --x          <int>    number of x pixels     (1040)
-y, --y          <int>    number of y pixels     (1392)
-c, --counts     <int>    counts                 (1)
-n, --noise      <int>    stddev of counts       (350)
-g, --gain       <float>  gain of sensor         (0.270000)
-r, --rdnoise    <float>  rdnoise electrons      (5.6)
-t, --exptime    <float>  exposure time          (600)
-b, --binning    <int>    binning symmetric      (1)
--xpixsz         <float>  x pixel size           (9.0)
--ypixsz         <float>  y pixel size           (9.0)
--mean           <float>  mean count for darks   (350)
--poisson        <bool>   apply poisson using -n value (False)  
-v, --verbose    <bool>   be verbose about work    (False)
"""

__author__ = 'Wayne Green'
__version__ = '0.1'

#############################################################################
#                                  Main
#############################################################################

if __name__ == '__main__':
    opts = optparse.OptionParser(usage='%prog ' + __doc__)

    opts.add_option('-x', '--x', action='store', dest='xpixels', 
                    default='1040',
                    help='<int>      number of x pixels default = 1040.')

    opts.add_option('-y', '--y', action='store', dest='ypixels', 
                    default='1392',
                    help='<int>      number of y pixels default = 1392.')

    opts.add_option('-c', '--counts', action='store', dest='counts', 
                    default='1',
                    help='<int>     counts  default = 1')

    opts.add_option('-n', '--noise', action='store', dest='noise', 
                    default='0',
                    help='<int>     stddev of counts  default = 0.')

    opts.add_option('-g', '--gain', action='store', dest='gain', 
                    default='0.277777',
                    help='<float>    gain of sensor   default 0.270000.')

    opts.add_option('-r', '--rdnoise', action='store', dest='rdnoise', 
                    default='5.6',
                    help='<float>     rdnoise electrons  default=5.6.')

    opts.add_option('-t', '--exptime', action='store', dest='exptime', 
                    default=600,
                    help='<float>       default=600.')

    opts.add_option('-e', '--exptype', action='store', dest='exptype', 
                    default='Bias Frame',
                    help="<str>     exposure type  default = 'Bias Frame'.")

    opts.add_option('-b', '--binning', action='store', dest='binning', 
                    default='1',
                    help='<int>     binning assumed symmetric  default = 1')

    opts.add_option('--xpixsz', action='store', dest='xpixsz', 
                    default='9.9',
                    help='<float>     x pixel size  default = 9.0')

    opts.add_option('--ypixsz', action='store', dest='ypixsz', 
                    default='9.9',
                    help='<float>     y pixel size   default = 9.0')

    opts.add_option('--mean', action='store', dest='mean', 
                    default='350.0',
                    help='<float>     mean count for darks  default = 350')

    opts.add_option('--poisson', action='store_true', dest='poisson', 
                    default=False,
                    help='<bool>     fake with noise proortional to noise.')

    opts.add_option('-v', '--verbose', action='store_true', dest='verboseflag', 
                    default=False,
                    help='<bool>     be verbose about work.')

    ###############################################################
    #  Gather the options
    #  
    ###############################################################
    options, args = opts.parse_args()
    NAXIS1        = options.xpixels
    NAXIS2        = options.ypixels
    EXPTIME       = options.exptime
    BINNING       = options.binning
    GAIN          = options.gain
    RDNOISE       = options.rdnoise
    EXPTYPE       = options.exptype

    ###############################################################
    #  Fix the types, etc ready to roll
    ###############################################################
    noise         = float(options.noise)
    naxis1        = int(NAXIS1)
    naxis2        = int(NAXIS2)
    binning       = int(BINNING)
    gain          = float(GAIN)
    rdnoise       = float(RDNOISE)
    exptime       = float(EXPTIME)
    xpixsz        = float(options.xpixsz)
    ypixsz        = float(options.ypixsz)
    mean          = float(options.mean)
    dateobs       = str.split(datetime.datetime.isoformat(datetime.datetime.now()), '.')[0]

    prefix = ''
    if( options.poisson != 0):
        data = guess = np.random.poisson(mean * np.ones((naxis1, naxis2)))
        #data[naxis1-1,naxis2-1] = 65535
        prefix = 'poisson_'
    else:
        data = np.ones((naxis1, naxis2))


    h = fits.Header()
    h.set('XBINNING' , binning   , 'Binning level along the X-axis')
    h.set('YBINNING' , binning   , 'Binning level along the Y-axis')
    h.set('GAIN'     , gain      , 'electrons / ADU')
    h.set('EXPTYPE'  , EXPTYPE   , 'Type of image')
    h.set('DATE-OBS' , dateobs   , 'YYYY-MM-DDThh:mm:ss observation start , UT')
    h.set('EXPTIME'  , EXPTYPE   , 'seconds')
    h.set('XPIXSZ'   , xpixsz    , 'Pixel Height in microns (after binning)')
    h.set('YPIXSZ'   , ypixsz    , 'Pixel Height in microns (after binning)')
    h.set('RDNOISE'  , rdnoise   , 'electrons ')

    f = fits.PrimaryHDU(data,h)

    if os.path.exists(prefix+'fakedark.fits'):
        os.remove(prefix+'fakedark.fits')

    f.writeto(prefix+'fakedark.fits')
