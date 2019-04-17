1. 學號：B04501095
2. 姓名：黃平瑋
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ -std=c++11>
5. 檔案壓縮方式: <zip>
6. 各檔案說明：
	 src/ 		: 程式碼
	 checker/	: 驗證程式
	 input_pa1  : 輸入資料
	 Report.doc : 程式報告
	 readme.txt : 編譯, 執行方法

7. 編譯方式說明：        	
	cd src/
	make clean
	make
   
8. 執行、使用方式說明：
   主程式：
   編譯完成後，在檔案根目錄下會產生一個 fm 的執行檔
   執行檔的命令格式為：
   ./fm <input file name> <output file name>
   ex: ./fm input_pa1/input_o.dat 0.out

   驗證程式：
   checker/linux_check <input file name> <output file name>    

9. 執行結果說明（說明執行結果的觀看方法，及解釋各項數據等）：
   主程式：
 	 // parse cells, nets
	 Finish parsing with 3000 cells and 5000 nets. maxGain:5.    
	 Finish initial partition. Criteria: 1485.00 - 1515.00

	 Iteration# 1
	 Finish init gain, cut size = 2964                               // 在iteration前的cut size
	 Largest Gain:1532 (at step: 1281) | A size:1515 B size:1485     // gain值 以及兩個partition size
	 




       

