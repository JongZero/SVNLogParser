# SVNLogParser

- github

![image](https://user-images.githubusercontent.com/97003592/163381337-69dd54eb-b3bd-46e9-9327-63c3eb3795df.png) 

- svn

![image](https://user-images.githubusercontent.com/97003592/163382489-3fa28144-e5a0-41ed-a659-5d05fae4e854.png) 

SVN 커밋 기록을 Github처럼 한 눈에 볼 수 없을까해서 만들어본 프로그램입니다. 엑셀을 이용해 만듭니다.

사용 프로그램 : tortoiseSVN, Excel, Visual Studio 2019



사용 방법은 다음과 같습니다.

1. svn의 Show log에서 뽑기를 원하는 커밋 기록을 Full revision data without changed paths로 클립보드에 복사하고, 메모장에 저장합니다.

![svn뽑기](https://user-images.githubusercontent.com/97003592/163383665-b1887034-408c-45e8-b3dd-da8a5737d6bd.png)


2. (1)에서 저장한 txt파일을 Data 폴더의 Original에 넣습니다. (이름 상관없고, 여러 개여도 됩니다.)

![폴더](https://user-images.githubusercontent.com/97003592/163383952-42277bf3-e265-4d37-b716-d97351ec010d.PNG)


3. exe 폴더에 있는 SVNLogParser.exe를 실행시킵니다. Data 폴더의 Converted 폴더에 ConvertedSVNCommitData.txt가 나옵니다.

![image](https://user-images.githubusercontent.com/97003592/163384318-bd20c3dd-6cd4-4399-86ae-b3768365f470.png)


4. 엑셀을 켜고 파일 열기에서 오른쪽 아래 모든 파일을 선택하고 ConvertedSVNCommitData.txt를 엽니다.
 
![image](https://user-images.githubusercontent.com/97003592/163384526-971328bb-8825-45db-988b-219bce383b55.png)


5. 설정을 건드리지 않고 다음 -> 다음 -> 마침을 눌러 텍스트 파일을 엽니다.

![image](https://user-images.githubusercontent.com/97003592/163384802-dd8c547c-dff7-41fd-aedf-6cbb71afa4cb.png)


6. #####으로 나온다면 A열의 크기를 늘려주면 됩니다. A1은 처음 커밋날짜, A2는 마지막 커밋날짜입니다.

![image](https://user-images.githubusercontent.com/97003592/163384929-c78779ed-deb9-4aa6-8c72-fc3b990c6b0c.png)
![image](https://user-images.githubusercontent.com/97003592/163384984-70b9ea29-d68f-428e-8666-b1316a9e67d3.png)


7. C3에 다음을 복사합니다. =INDEX($A$1:$A$2000,ROW(C3)+(7*(COLUMNS($C$3:C$3)-1)))

복사 후, 셀 서식을 날짜로 바꿔줍니다.

![셀서식](https://user-images.githubusercontent.com/97003592/163385467-46dddf12-3c42-40c7-9300-b1c509e6b481.png)
![셀서식2](https://user-images.githubusercontent.com/97003592/163385471-d4fa1f2f-2934-48ac-a0a3-2057b37d162a.PNG)


8. 열의 크기를 줄입니다.

![열줄이기](https://user-images.githubusercontent.com/97003592/163385936-f38ff07a-9153-4541-ae2d-284cd7254349.png)
![열줄이기2](https://user-images.githubusercontent.com/97003592/163385962-cfa06dee-fb2f-4a9d-8d46-9216d76488a9.PNG)


9. A3을 9행까지 끌어 복사합니다.

![9행까지복사](https://user-images.githubusercontent.com/97003592/163386194-479bec64-678f-44e5-bd26-b6bf82cd7f3e.png)


10. A3 ~ A9를 오른쪽으로 끌어서 복사합니다. 여유롭게 복사합니다.

![행복사](https://user-images.githubusercontent.com/97003592/163386470-6a2ae40c-c705-4593-9c4a-a3dc1697d454.png)


11. 방금 블록까지 드래그 후 조건부서식 - 셀 강조 규칙 - 보다 큼

![조건부서식](https://user-images.githubusercontent.com/97003592/163386687-7fe46391-1399-4c07-8935-a9361261dc1b.png)


12. 사용자 지정 서식으로 색을 지정합니다. 글씨 색과 채우기 색을 동일하게 합니다.

색이 바뀐 것들이 커밋한 날짜입니다.

![커밋완료](https://user-images.githubusercontent.com/97003592/163387301-27b24793-206d-4a0e-96f9-776199215b9e.PNG)


13. 마찬가지로 조건부서식 - 셀 강조 규칙 - 보다 작음을 하면 커밋을 하지 않은 날짜를 강조할 수 있습니다.

14. 테두리도 넣고 꾸미면 완성!

![svn커밋기록](https://user-images.githubusercontent.com/97003592/163387610-4c023a63-129d-4eb9-852e-55e2f106192c.PNG)
