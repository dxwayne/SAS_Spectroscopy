# Tuscen Camera Evaluation

<pre>
Wayne 2019-12-07
The DICOM image is like a FITS MEF file, in that it contains n blocks,
each block with a metadata chunk and an data chunk -- an image array
may be a data chunk.  This format is "Digital Imaging and
Communications in Medicine" or DICOM.  The idea started in 1983, and
was most recently expressed in 2004 for medical images and 2009 for
TSC/airport use. So it is well understood and well documented.

TODO: Determine any overscan or trim areas in the iamge and add
keywords as needed for these.

TODO: Answer if we want to store the original DICOM header(s) into a
separate table in the fits file as an extent?

TODO: What do headers mean and which ones to use?


Anaconda's python has a library for working with the DICOM files.

Standard FITS header values (seldom standard). I probably missed
a few off the top of my head: Should include:

CATALOG    [name of the database ]
OBJECT     [database main id prose ]
OBJECTID   [key for database ]
IMAGETYP   one of ['zero','dark','flat','domeflat','skyflat','focus' ]
FILTER     [prose name of filter at site ]
CCDSUM     [nx ny [residualx,residualy]] if n=3 and NAXIS mod n has a residual
           residual can be stated. (IfA)
OBSERVER   OBSERVxx [ name of the observer(s)
SITE       [prose of telescope/observatory name ]
RA         [sexigesimal ra ]
DEC        [sexigesimal dec ]
RA-TARG    [decimal degrees ra ]  -- easier to work with
DEC-TARG   [decimal degrees dec ]
ROTANG     [rotation in degrees + is CCW range [0..360) ]
RADESYS    ICRS
DATE-OBS   [ YYYY-MM-DDTHH:MM:SS.SSSsssZ  start of obs]
DATE-MJD
TIMESYS    [ UTC (or other) ]
EXPTIME    [ time detector accepting photons ]
GAIN
RDNOISE (may be subjective)
INSTRUME  [ prose of the instrument [site s/n]]
TELESCOP  [ prose of the telescope name ]
APERTURE  [ area of effective aperature in m-squared ]
FOCALLEN  [ effective focal length with barlow/reducer ]
CAMERA    [ prose of camera ]
DETECTOR  [ prose of the detector ]

Data sections: Some CCDs have more than one readout circuit. This
has significant implications in noise frames.
DATASEC[n] [data array (pixel) coordinates ]
CCDSEC[n]
DETSEC[n]
AMPSEC[n]



I prefer OBSGEO, uses  IAU 1976 ellipsoid
XDATUM                      [ prose of the datum for geo coords ]
OBSGEO-B Latitude           [ degrees ]
OBSGEO-L Longitude          [ degrees ]
OBSGEO-H Height above datum [ meters ]

BUT! our gps's use WGS-84, and that will change real soon.
All this can be handled in Astropy.

Other things like
AIRMASS
MOONANGL   [ degrees ]
SUNANGLE   [ degrees ]
EXPSTART   [ start of exposure in MJD ]
EXPEND     [ end of exposuire in MJD ]

ROOTNAME   [ root name of the file ]
FILETYPE   [ prose 'CALIBRATED IMAGE' or 'RAW IMAGE' ]

Other things like WCS
(impose a fake wcs or a pinpoint solution?)

</pre>
