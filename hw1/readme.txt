1. 學號：B04501095
2. 姓名：黃平瑋
3. 使用之程式語言：< C++ >
4. 使用之編譯器：< GNU g++ -std=c++11>
5. 檔案壓縮方式: <tar>
6. 各檔案說明：
	 src/ 		: 程式碼
	 checker/	: 驗證程式
	 input_pa1  : 輸入資料
	 Report.doc : 程式報告
	 readme.txt : 編譯, 執行方法

7. 編譯方式說明：        	
	cd src/
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
 	 Program initializing
	 Top cell name: V07Q						-- 頂層元件名稱
	 Max hierarchical level: 12					-- 最大階層數
	 Real rects count = 398863					-- 幾何圖形數
	 Start of overlapping checking
	 End of overlapping checking
	 Start of exploding
	 Finishing program and writing result
	
	
	 Program initialization spends 0.420000 seconds			-- 各項步驟計時
	 Overlapping graph construction spends 65.780000 secnods
	 Removing of hierarchical overlapping spends 0.700000 secnods
	 Finishing program and writing results spends 2.600000 seconds

	 Total program time = 69.500000 seconds				-- 程式總執行時間



       

