## Meeting 1/12/2021

Issues:

> Dicom SCU export from modality

Add specific date range

> Full dicom/worklist **logging locally** via log.txt

Configurable Error logging (max level)

> Evaluate Worklist GUI



> Dicom Config

> Testing issues (?)

Questions:

- Is any field guaranteed to be unique?
- What is accession and how is it used?

TODO:
- Get Conformance document


## Email from Kaiser (1/5/2021)

|From|Bryan L Gros <Bryan.L.Gros@kp.org>|
|:-|:-|
|Sent| Tuesday, January 5, 2021 11:18 AM|
|To| Ray Sievert <rsievert@traceytech.com>; Andrea Gatz <agatz@traceytech.com>; Sandeep Singh <Sandeep.Singh@kp.org>; Dominic M Leung <Dominic.M.Leung@kp.org>|
|Cc| Tommy Bothwell <Tommy.Bothwell@kp.org>|
|Subject| iTrace DICOM progress|

Happy New Year

Ray, Andrea, I wanted to schedule a meeting early next week to review current status on the DICOM functionality update.  Here’s the notes I sent about a month ago:

Ray, Andrea, thanks for your time in helping up test out the software.

Here’s the feedback that I got back. Is this sufficient to improve the functionality?

I think number 1 is the biggest ask—supporting the full DICOM SCU standard.

1. Dicom SCU export from modality
2. Full dicom/worklist **logging locally** via log.txt
3. Remove the “1 order found” prompt and replace with a full blown Worklist GUI (Worklist Order screen) ..similar to OCT, Lenstar, etc.
4. Move all the Dicom/Worklist settings to a separate ‘Dicom Config’ tab within the user interface

After importing the studies that we did on December 17, here is additional feedback.  I am not sure that I sent this to you last year.

I’ve finished importing those DICOM files into our QC tools. There are two major issues with the DICOM files produced by iTrace.

- iTrace is using illegal character in series unique ID (UID). UID can only accept 0 to 9 and the character period “.”. From the attached screenshot you can see that it’s using alphanumeric as well as special characters in its series UID. As a result of this implementation the files are not accepted by our image archive (eMIA). Files are accepted by Merge, though.
- iTrace is using duplicate series UID in “Wavefront” and “Chang” images. This is similar to the issue with Lenstar, where there are duplicate series UID (different study UID but identical series UID) for either the same or different patients. Only half of the images of each patient can be imported into ECP as a result of this error. iTrace should use the same study and series UID for both the Wavefront and Chang images since they are related

## Email from Eric Anderson 1/5/2021)

|From|eandersen@intrapulse.com|
|:-|:-|
|Send|Jan 5, 2021, 11:55 AM|
|to|Andrea, me|

Hi Thanos,

The fields required (and optional) are determined by the SOP/IODs you are creating exclusively.

For example you are creating a Secondary Capture SOP, so you have to fulfill the requirements for the IOD module

> http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_A.8.html

The  General Study IOD includes the Accession Number for example which is from the worklist item, which must be included, but can be empty (type 2)

> http://dicom.nema.org/medical/dicom/current/output/chtml/part03/sect_C.7.2.html#sect_C.7.2.1

There can be additional requirements by Kaiser however – which should be listed in their conformance statement.

You can send an unsolicited SOP to their server – the worklist fields would be set as required (empty or not included) as per the Secondary Capture IOD rules, but again they may have additional requirements, which should be specified in their conformance statement (or perhaps elsewhere).

Note that I wouldn’t allow the user to edit data that comes from the worklist server (name, id, etc).