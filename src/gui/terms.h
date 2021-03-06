
1.  A DLL file named “thw.dll” should bethe one to control and check the status of iTrace analyzer. 

A: This file is produced by the designer and manufacturer of the controlboard in the iTrace hardware. 
They also wrote the code that resides in the onboard processor.  
This company is called Indus Instruments.  
We have a good relationship with them.

2.  
CIOL:  IOL stands for Intra Ocular Lens, which is the plastic lens put in the eye during cataract 
	   surgery.  I suspect this class has something to do with the toric planner code.

CICL:  ICL stands for Intra Ocular Contact Lens.  
	   This is like a standard contact lens, except they put it inside the eye 
	   (behind the iris and in front of the natural lens).  
	   In cataract surgery they take out the natural lens and replace it with an IOL.  
	   For ICL surgery they leave the natural lens in place and just put in the ICL to
	   correct refractive errors.  
	   There is a new software module for planning ICL surgery that works like the toric planner. 
	   It is in the 5.1 code,but I think it is not accessible for use.  
	   Sergey was meant to have made 5.1 so that this feature can be easily turned on when the 
	   testing is complete.  You will need to work out what he did here.

CLAD:  I think LAD stands for Linear Array Detector.  
	   This is the main hardware sensor of the iTrace that detects the laser light reflected off 
	   the retina at the back of the eye.  There are two of them.  
	   These give the raw data for the WF measurements.

CPSF:  PSF is Point Spread Function.  // 点扩散函数，就是光学系统的脉冲响应函数 
	   This is one of the display methods (the one I showed you early on that I like to put in 3D).

CMTF:  MTF is Modulation Transfer Function.  
	   This is another of the display methods.  
	   It is related to the PSF in that the MTF is the rotationally averaged 2D Fourier Transform 
	   of the PSF.

	   
3.  What do you think the ‘Compensation Motor’ in the code is most likely referred to?

A: There is only one motor in the system.  
It moves the position of the fixation target (red light the patient looks at) to compensate for 
their refractive error.  
This set during the WF measurement process. 
It sets it at a starting position that is just beyond infinity. 
Then if fogging is set to on, the system does one quick WF measurement to get the refractive 
error of the patient, and moves the light again to a position that is calculated from the patients 
refractive error.  The very first WF measurement also has the motor driven from one end of the range 
to the other to calibrate its position.

4. There are some definitions in the code as following:

“WAVETOUCH_LENS_BASECURVE _UNKNOWN   

WAVETOUCH_LENS_BASECURVE _MEDIAN    

WAVETOUCH_LENS_BASECURVE _STEEP     

WavetouchLensPower”

A: Ask Andrea for some background reading on WaveTouch lenses.  
This is a sister company of Tracey Technologies.  They make custom contact lenses.  A trial lens is 
fitted to the patient and a WF map is taken to measure the remaining aberrations.  The "BASECURVE" 
is the curvature of the back surface of the trial contact lens.  The iTrace code also locates 3 dots 
on the trial lens, so that its position on the eye can be measured, so the customized contact lens 
is adjusted in the right area.

5.
What  does the following conceptsmean?

‘Higher Order Total, or Lower Order Total’ 

A: The RMS values of the wavefront errors can be divided into high order and low order.  
   Low order is basically sphere and astigmatism. High order is every aberration higher than 
   astigmatism.  The totals are the sum of these RMS values.  
   I can't recall if this is a simple sum (RMS is root mean squared) or if the numbers must be 
   squared then added then take the squareroot.


 ‘K1 does,D/ K2,D/K2 axis for the keratomertery’ ,

A: I assume these are simulated Keratometry values.  
   The keratometer is a more basic device than a topographer.  
   But it has been used in ophthalmology for a 100 years.  
   The values are reported in Diopters(D).  
   This is the inverse of the focus length in meters.  
   K1 and K2 might be steep and flat K readings (I am not sure).


‘fogging’ , ‘Local ROC inside’

A: "fogging" is the process of driving the fixation light so that it is optically just beyond 
   infinity for the patient.
   ROC is the radius of curvature.  Local ROC is one of the topography maps.  

6. Trefoil abberations
*****

7. IR DOT
A: Infrared (IR) dots
There 4 Infrared dots in the center of the pupil, which is used to DAU to capture the pupil.

8. DAU
A: data acquisition unit

9. Send AR/KR
A: If Show “Send AR/KR” button is checked, the software will display the button that
enables the process of sending the auto-refraction and keratometry data from the iTrace
to a Topcon CV-5000 automated lane system. If the iTrace is not connected to a lane
system, this checkbox can be left unchecked.

10. UCVA/BCVA
A: UCVA is uncorrected visual acuity or how well you can see without any glasses or contact lenses.  
   This could be estimated from the total wavefront error.
   BCVA is best corrected visual acuity or how well you see with the best glasses for you.  
   This could be estimated from the HO (higher order)wavefront total (which has the sphere and 
   cylinder that would be corrected in the glasses removed).

11. 'Visual Acuity chart' (new name in 5.1.0) is equal to the 'Snellen letter'(original in 5.0.1)
******

12. Surgery induced corneal astigmatism (SIA) = cyl power 
******

13. Vertex distance
   The distance from the glasses to the center of pupil

14. Fixation target (Badal Optometer)

15. Zernike RMS data
******

16. Angle Kappa and Angle 
A: Angle Kappa - The distance between the center of the pupil and visual axis.
   Angle Alpha - the distance between the center of the limbus and the visual axis.

17. RSD Display
A: The RSD (Retinal Spot Diagram) is shown at bottom left of screen(WF Verification Display). 
   This image showed a good data acquisition.
   A RSD with severely skewed points may indicate an error in the scan or an
   extremely aberrated eye. Either way, the exam should be retaken to confirm the analysis.

18. Sim K - Simulated Keratometry

19. Rabinowitz Keratoconus test
**********

20. VFA
A: Visual function analysis

21. Keratoconus

22. 
