
#run1.sh test
## This is to run the first program SAT.java

if [ -e "SAT.class" ]; 
then
    java SAT $1
else
    echo "First run build.sh to compile the code"
fi
