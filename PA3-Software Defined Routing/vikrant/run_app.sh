#~/Downloads/mussh/./mussh
~/Downloads/mussh/./mussh -h vikrant@embankment.cse.buffalo.edu -c "cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; make clean all"
~/Downloads/mussh/./mussh -h vikrant@underground.cse.buffalo.edu -c "cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; make clean all"
~/Downloads/mussh/./mussh -h vikrant@stones.cse.buffalo.edu -c "cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; make clean all"
~/Downloads/mussh/./mussh -h vikrant@euston.cse.buffalo.edu -c "cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; make clean all"
~/Downloads/mussh/./mussh -h vikrant@highgate.cse.buffalo.edu -c "cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; make clean all"


#~/Downloads/mussh/./mussh -h vikrant@embankment.cse.buffalo.edu -c "mkdir /local/Fall_2017/vikrant/grader-cse489589_assignment3"
#~/Downloads/mussh/./mussh -h vikrant@underground.cse.buffalo.edu -c "mkdir /local/Fall_2017/vikrant/grader-cse489589_assignment3"
#~/Downloads/mussh/./mussh -h vikrant@stones.cse.buffalo.edu -c "mkdir /local/Fall_2017/vikrant/grader-cse489589_assignment3"
#~/Downloads/mussh/./mussh -h vikrant@euston.cse.buffalo.edu -c "mkdir /local/Fall_2017/vikrant/grader-cse489589_assignment3"
#~/Downloads/mussh/./mussh -h vikrant@highgate.cse.buffalo.edu -c "mkdir /local/Fall_2017/vikrant/grader-cse489589_assignment3"
#~/Downloads/mussh/./mussh -b -h vikrant@underground.cse.buffalo.edu -c "/usr/sbin/lsof -i tcp:4491 | awk 'NR!=1 {print $2}' | xargs kill; cd /local/Fall_2017/vikrant/cse489589_assignment3/vikrant/; ./assignment3 4491"

#Kill the previous process be a good citizen
#https://stackoverflow.com/questions/5043808/how-to-find-processes-based-on-port-and-kill-them-all
#~/Downloads/mussh/./mussh -a -d 2 -H routers -c "/local/Fall_2017/vikrant/cse489589_assignment3/vikrant/./assigment3.sh"
