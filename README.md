# Fall Semester Final Report

## Results

### Experiments
We were able to design and implement two experiments, but they are still work in progress. Each experiment is concerned with depth perception, so they could be performed in four preliminary conditions: fixed camera, planer camera motion, planar camera motion with look at rotation, fixed camera location with rotation. Each of these conditions could be applied with or without stereoscopic vision (Look at VRPawn.cpp updater function to see the implementation of the motions). 

#### Closer Object Experiment
This experiment presents the subject with two cubes and asks which one is closer to the camera. The user then clicks on the one that they think is, and then the distance between the objects changes according to their response. The distance changes in a way that enables a staircase experiment, so it gets smaller if the answer is correct and it gets larger if it isn’t. Overtime, the distance converges to the true threshold, which is the thing we want to measure. We were only able to perform the conditions in mono-scopic vision, and here is the ordering that we found (higher is better): 
1.	Planar motion with lookat
2.	Planar motion
3.	Fixed camera
4. Just rotation

#### Falling Object Experiment
In this experiment, the participant is given a motion controller and asked to catch a falling ball on a table. All potential depth cues that are uncontrolled for, such as shadows, have been eliminated except for perspective. The motion controller controls a disc with fixed height, so the participant can’t catch the ball early. (Look at Work for further discussion about this). We were only able to run this experiment with two conditions, also in mono: Fixed camera, Planar motion with lookat. The results were as we expected which is the moving camera produced better scores than the fixed camera one. 

### Camera Solver
Another aspect that I explored in this project is figuring out camera motions that best induce parallax. I had little time to play around with this, but I think that I was able to produce some interesting results. The solver uses a method that Danny suggested. The method is as follows: which point in space that if the camera moves to will produce the greatest displacement of objects in image space. I implemented this in Python and manually moved the data (such as object positions) from Unreal to the script and vice versa (I did not explore automatic ways of doing this, but I’m sure that plenty exist). The environment that I ran the script in was abstract and simplistic, but the results were promising. The solver chose for the camera to go to points that provided a lot of contrast in the positions of the objects in image space. The code for this could be seen in the notebook in (Scripts) and the map in unreal is called ParallaxMap.

## Work

### Experiment Design
There were a lot of back and forth when it came to designing and implementing the experiments. A lot of work went into reiterative design where I would add something to one of the experiments, try it out, and then realize that it might work better if I do that instead. There were too many instances of this, and I don’t think they are worth mentioning. The falling object experiment, however, gets its own section about control frame issues. 

#### Falling Object Control Frame
A big issue that we encountered was that when the camera changes rotation, the forward axis that the participants see is misaligned with the one the controllers use. To mitigate this, we tried implementing a solution where the disk matches the instantaneous velocity of the controller rather than its position. Not a lot of time was put into this, but it seemed nontrivial to get right, so we decided to change the experiment itself for the time being. We made to versions of this, the first is a one-dimensional analog to the original experiment where the participant can only move a long rectangle in one dimension using the mouse. This eliminates the issue of adjusting for the forward axis with rotation. The second is the same as the first but the input device is a mouse instead of a motion controller. The reasoning behind this is that participants will be able to ground the mouse motion to the table that it sits on which partially resembles the table the balls fall on. Experimentations to validate there two alterations are yet to be made. 

### IRB Application
Quite a bit of my time went into getting the IRB application done. This was my first time filling out an application of this sorts, so it took me a lot longer than I expected. I was able to meet with people from the IRB office and talk to them about the application, which was helpful. Also, I created a consent form for the participants to attach to the application.

### Motion Solver
I initially tried to link NLOpt to Unreal Engine 4 and use its functions directly in C++. Even though I got close to getting it to work, we decided that it would be better to have an external script that solves the motion and send it back to the engine. This meant that I had to implement Unreal’s projection function in the script for the solver to use. Once I got the projection function to work, I moved to implement the objective functions which calculated the displacement amount of a point in 3D space from the starting point. When I got that working by cross validating the results with a similar function written in Unreal, I optimized it using NLOpt and got the results that I talked about earlier.

### Research
In parallel to the things previously mentioned, I spent some time looking up and reading research papers related to the work we are doing. A list of the things is under (Bibliography).

## What I Learned 

### Technical Skills
- Reading research papers: I had read technical research papers before, but there was always a structure to how I read them (i.e. certain expectations/points to get out of them). This term, however, I had to first find my own papers and then read them to get the specific things that I’m looking for which was a skill that I did not have before. 
- Optimization: I got a taste for optimization using NLOpt in the short period that I worked on the script in. I still have a lot to learn, but I feel like I learned enough to do interesting applications with it.
- Experiment design: If anything, I at least learned that experiment design is *hard* and why. 

### Non-Technical
- Time management: doing this project while taking classes was a little rough. There were weeks where I was swamped with work and other where I had a lot of free time. As the semester progressed, I felt like I was able to manage my time better by trying to achieve a uniform load distribution.
- Working under expectations and deadlines: when I work on open ended projects, I usually do them for myself, so I rarely have people who are expecting to see results. This project gave me a chance to experience that firsthand which was a pleasant change to be honest.

## Self-Evaluation
I’m happy with the work that I’ve done this semester. I feel like the work I did was more of an exploratory journey, which honestly yielded some pleasant outcomes. There are things that I wish I did differently such as managing my time from the start of the semester, but at least I’ll start the next semester stronger than this one. I also wished that I spent more time developing the motion solver, but given that this is a research project, certain things had to be prioritized to move things forward.
Other than the things I did, I’m quite happy with what I got out of this in terms of personal and professional development. I feel like I gained very important skills that would have taken more time to learn in other settings.

## Bibliography
Gerig, N., Mayo, J., Baur, K., Wittmann, F., Riener, R., & Wolf, P. (2018). Missing depth cues in virtual reality limit performance and quality of three dimensional reaching movements. Plos One, 13(1). doi: 10.1371/journal.pone.0189275
	This paper was a good read at the beginning of the project where I was just collecting information. In short, what they found is that both stereopsis and parallax add depth perception cues. It was a useful find because it gave us proof that parallax does indeed work and there is some merit in trying to explore it a little further.


Volcic, R., Fantoni, C., Caudek, C., Assad, J. A., & Domini, F. (2013). Visuomotor Adaptation Changes Stereoscopic Depth Perception and Tactile Discrimination. The Journal of Neuroscience, 33(43), 17081–17088. doi: 10.1523/jneurosci.2936-13.2013
	The authors discussed how people perceive depth differently and they tend to have a preferred distance at which they judge depth most accurately given all the usual visual cues. What they found is that people overestimate depths when objects are closer and underestimate depth when objects are further away. The sweet distance is arm length which is different for every person. Looking back, I don’t think we used this outcome at all, but it could be a thing to control for especially with different participants.


Hibbard, P. B., Haines, A. E., & Hornsey, R. L. (2017). Magnitude, precision, and realism of depth perception in stereoscopic vision. Cognitive Research: Principles and Implications, 2(1). doi: 10.1186/s41235-017-0062-7
	I did not read the entirety of this paper, but it talks about stereo in headsets. What they found is that the greater the disparity between the images presented to each eye, the better we are at perceiving depth, but also lose other important things.  


Kellnhofer, P., Didyk, P., Ritschel, T., Masia, B., Myszkowski, K., & Seidel, H.-P. (2016). Motion parallax in stereo 3D. ACM Transactions on Graphics, 35(6), 1–12. doi: 10.1145/2980179.2980230
	What the authors of this paper did line up with many of the experiments that we will be conducting, however they’re main goals is different than ours. They seem to be extracting parallax depth information from videos and reinserting them as binocular disparity cues. Regardless, the related work section and experiment design is insightful.

## Files
[Unreal Project files](RobotControl/README.md)


[Python Scripts](scripts/README.md)
