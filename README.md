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
