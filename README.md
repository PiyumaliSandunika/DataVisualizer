# DataVisualizer
This project is to analyze multiple  data files (.csv) and visually represent the information as the user may require

usage: 

./executedFileName [-l length] [-m | -t | -p] [--scaled] filename1 filename2 ..

length - Number of entries to display 
-m    ->  data sorted according to number of meeting
-p    ->  data sorted according to number of partcipants
-m    ->  data sorted according to amount of time

--scaled  --> to occupy the maximum printing width

files should be .csv type   ex: sampleData1.csv sampleData2.csv

default will be length of 10 and data sorted according to the number of meeting. 

./executedFileName csvfilename.csv

![Screenshot (2)](https://user-images.githubusercontent.com/73444543/185036544-c1bec73b-1d7c-4dd1-98ff-f897cc759c08.png)
