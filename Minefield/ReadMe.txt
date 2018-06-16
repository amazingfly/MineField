========================================================================
    CONSOLE APPLICATION : Minefield Project Overview

	ZOS Software Engineer Applicant Test
========================================================================

This application consists of a large number of mine objects that have a unique identifier, a position,
a team and can be marked as active or invulnerable.

Each loop, all active mines find what other non-invulnerable mines are within their blast radius
(i.e. what other mines would be damaged if it exploded).  Then, for each team, the mine with the most targets is exploded.

This continues until no more mines can be exploded as they will not do any more damage.

The team with the most remaining mines is declared the winner.

Tips:
-start by making sure the code compiles cleanly
-get the code to run (it does not run successfully right off the bat)
-verify that the code is accomplishing its main goal correctly
-look for ways to optimize the code for both performance and memory usage
  * there are significant gains that can be made in performance
-look to enforce defensive programming (const correctness, error checking and handling, etc.)
-invulnerable mines cannot take damage from other mines
-invulnerable mines are not targetable
-inactive mines cannot be triggered to explode. They can only explode from taking too much damage
-even a mine with no targets can be triggered to explode

Please comment changes and alterations to the code.

Please also feel free to comment on your thought process or alternate attempts you made when
modifying any of the code.

Try to stick to the code convention that you find in the existing code.  Feel free to fix
any inconsistencies.

Extra bonus tasks:
* Remove the need to use cMaximumNumberOfObjects in ObjectManager.h
* Change the best mine to explode check to one of the following:
  - Mine that will do the most cumulative damage
  - Mine that causes the most number of enemy mines to die
  - Mine that maximses damge to enemy mines but does least amount of damage to friendly mines
  - Variants of all three of the above that include recursive checks on other mines that explode 
    and cause secondary damage
* Is there a better way to stall the main thread waiting for all the worker threads to finish?
* Write a test harness that allows multiple checks of various numbers of worker threads, teams
  and mines per team given a random seed
  - Test harness could be used to compare run times as teams or number of mines changes
  - Or test which mine explode check (if one of the above is written) results in the most destruction 
  - If a test harness is added, provide an additional command line option that allows it run while
    preserving the existing command line options

Make sure that your name, the date you did this test and the amount of time taken are marked
in Minefield.cpp.

Getting the exact same results is not a requirement to ‘pass’ the test. We are far more interested
in code fixes, cleanup, style, improvements, issues uncovered, performance considerations, etc.

Usage of C++03 or later is permitted.

It is preferred that Visual Studio 2010 or later is used as your development environment.  Visual Studio
Community 2015 can be downloaded free from:
http://www.visualstudio.com/en-us/downloads/download-visual-studio-vs.aspx
If another development environment is used please provide details.
Note: If you're coding on a non-Windows platform you'll have to replace the Mutex used.

Note any and all performance tools and techniques used. 

Do not modify the existing four command line input parameters used.  They will be used by a
test harness to check the output of your work.

The four command line parameters are: <random seed> <number of worker threads> <number of teams> <number of mines per team>

To be able to verify that you are getting the correct results, here are some sample results that show
you have the code accomplishing its main goal correctly:

With the following input parameters: 987654 1 3 500
Object id 1071 position (-755.938, 93.133, -556.052) active Y invulnerable N
Object id 275 position (266.852, -32.735, 970.941) active Y invulnerable N
Object id 934 position (570.726, -275.077, -346.969) active N invulnerable N
Object id 1863 position (903.713, -300.514, 338.788) active Y invulnerable Y
Object id 3885 position (331.019, 376.530, 101.836) active Y invulnerable N
Object id 1470 position (-924.728, 20.487, -262.222) active Y invulnerable N
Object id 3201 position (379.957, 81.881, 458.886) active Y invulnerable N
Object id 2538 position (-119.712, -5.246, -345.135) active Y invulnerable N
Object id 455 position (-610.912, -547.901, -254.453) active Y invulnerable N
Object id 2465 position (429.587, -295.623, -74.395) active Y invulnerable N
Team 0 has 440 mines remaining
Team 1 has 400 mines remaining
Team 2 has 357 mines remaining
Team 0 WINS after 36 turns!!

With the following input parameters: 456 16 3 1000
Object id 5241 position (-502.488, 960.968, -673.866) active Y invulnerable N
Object id 8308 position (-429.672, 251.257, -690.690) active Y invulnerable N
Object id 9649 position (518.235, 635.312, -637.790) active Y invulnerable N
Object id 5054 position (-410.384, -226.540, -217.858) active Y invulnerable Y
Object id 2728 position (373.186, -937.465, -62.392) active Y invulnerable N
Object id 8632 position (-651.196, 446.681, 638.653) active Y invulnerable N
Object id 5008 position (893.620, 164.387, 108.439) active Y invulnerable N
Object id 7782 position (-93.770, -602.303, 213.354) active Y invulnerable Y
Object id 4858 position (-599.576, 697.088, -730.733) active Y invulnerable N
Object id 507 position (698.314, 716.660, 750.904) active Y invulnerable N
Team 0 has 806 mines remaining
Team 1 has 738 mines remaining
Team 2 has 645 mines remaining
Team 0 WINS after 113 turns!!

With the following input parameters: 654321 12 5 1500
Object id 8591 position (-898.894, -290.149, 50.030) active Y invulnerable N
Object id 7110 position (-303.245, 718.965, -516.425) active Y invulnerable N
Object id 1466 position (637.269, -285.511, -88.674) active Y invulnerable N
Object id 10286 position (-312.755, -866.798, -53.850) active Y invulnerable N
Object id 10766 position (316.465, 415.601, 571.089) active Y invulnerable N
Object id 3469 position (956.990, -160.174, -141.135) active Y invulnerable N
Object id 44 position (447.428, -995.504, 571.503) active N invulnerable N
Object id 14905 position (-256.135, -394.987, -613.572) active Y invulnerable N
Object id 14267 position (-446.510, -544.947, 657.727) active Y invulnerable N
Object id 7000 position (-123.204, -670.820, 631.414) active Y invulnerable N
Team 0 has 1008 mines remaining
Team 1 has 823 mines remaining
Team 2 has 750 mines remaining
Team 3 has 670 mines remaining
Team 4 has 573 mines remaining
Team 0 WINS after 315 turns!!

You'lll want to make sure using the same random number generation and mutex code provided to ensure 
your results match the above.

These results were obtained on a Windows 10 PC.


/////////////////////////////////////////////////////////////////////////////
