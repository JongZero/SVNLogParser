#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

// 월마다 몇 일씩 있는지
const int g_MonthEndDayTable[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

struct Date
{
	int Year;
	int Month;
	int Day;
};

enum class Type
{
	eYear,
	eMonth,
	eDay,
};

// 중복 체크
bool CheckOverlap(const Date& d1, const Date& d2)
{
	return ((d1.Year == d2.Year) && (d1.Month == d2.Month) && (d1.Day == d2.Day));
}

// 정렬 함수
bool FuncSort(const Date& d1, const Date& d2)
{
	if (d1.Year == d2.Year)
	{
		if (d1.Month == d2.Month)
		{
			return d1.Day < d2.Day;
		}
		else
		{
			return d1.Month < d2.Month;
		}
	}
	else
	{
		return d1.Year < d2.Year;
	}
}

// 숫자인가?
bool isNum(const char& c)
{
	return (c >= '0' && c <= '9');
}

// C++ 17, 파일의 경로와 이름을 저장한다.
void LoadFilePathAndName_Recursive(std::wstring folderPath, std::unordered_map<std::string, std::string>& map, int& totalNum)
{
	const std::filesystem::path _folderPath{ folderPath.c_str() };
	std::filesystem::create_directories(_folderPath);

	for (const auto& file : std::filesystem::recursive_directory_iterator{ _folderPath })
	{
		std::wstring filePathW = file.path();
		std::string filePath(filePathW.begin(), filePathW.end());
		std::string fileName = filePath.substr(filePath.rfind("/") + 1, filePath.rfind("."));

		// 확장자 제거
		fileName = fileName.substr(0, fileName.size() - 4);

		// file path & file name 저장
		map[filePath] = fileName;

		totalNum++;
	}
}

// Date를 스트링으로 돌려줌 (2022,3,15 -> 2022-03-15)
std::string MakeDateStr(const Date& date)
{
	std::string str = std::to_string(date.Year);
	str += "-";

	if (date.Month < 10)
	{
		str += "0";
	}

	str += std::to_string(date.Month);
	str += "-";

	if (date.Day < 10)
	{
		str += "0";
	}
	
	str += std::to_string(date.Day);

	return str;
}

int main()
{
	std::vector<Date> dateVec;		// 일자들이 들어갈 벡터
	std::unordered_map<std::string, std::string> filePathAndNameUMap;	// 파일 경로와 이름
	int totalNum = 0;	// 총 개수
	int count = 0;		// 현재 읽은 개수

	LoadFilePathAndName_Recursive(L"../Data/Original/", filePathAndNameUMap, totalNum);

	std::cout << "===================================\n";
	std::cout << "SVN Commit Log Converter v0.1\n";
	std::cout << "2022. 03. 20\n";
	std::cout << "Made By JongZero\n";
	std::cout << "Total File Num : " << totalNum << '\n';
	std::cout << "===================================\n\n";

	std::cout << "Converting... (" << count << " / " << totalNum << ")\n";

	for (const auto& it : filePathAndNameUMap)
	{
		std::ifstream file(it.first);

		std::string str;

		Type type = Type::eYear;

		count++;

		std::cout << "Converting... (" << count << " / " << totalNum << ")\n";

		// 한 줄씩 읽으며 저장
		while (std::getline(file, str))
		{
			if (std::string::npos == str.find("Date"))
				continue;

			Date date;
			for (int i = 0; i < str.size();)
			{
				char nowChar = str[i];

				// 숫자일 경우
				if (isNum(nowChar))
				{
					// 숫자를 파싱하고, 다음에 파싱할 숫자의 타입을 갱신
					// Year
					if (type == Type::eYear)
					{
						date.Year = std::stoi(str.substr(i, 4));
						type = Type::eMonth;
						i += 4;
					}
					else
					{
						int offset = 1;
						// 다음 글자도 숫자일 경우
						if (isNum(str[i + 1]))
						{
							offset = 2;
						}

						// Month
						if (type == Type::eMonth)
						{
							date.Month = std::stoi(str.substr(i, offset));
							type = Type::eDay;
						}
						// Day
						else
						{
							date.Day = std::stoi(str.substr(i, offset));
							type = Type::eYear;
						}

						i += offset;
					}

					// Day까지 파싱이 되었을 때
					// 벡터에 넣고 초기화
					if (type == Type::eYear)
					{
						dateVec.emplace_back(date);
						break;
					}
				}
				else
				{
					i++;
				}
			}
		}
	}

	// 오래된 순부터 정렬
	sort(dateVec.begin(), dateVec.end(), FuncSort);

	// 중복되는 날짜 제거
	dateVec.erase(std::unique(dateVec.begin(), dateVec.end(), CheckOverlap), dateVec.end());

	// 시작 년/월/일
	Date startDate = dateVec.front();

	// 끝 년/월/일
	Date endDate = dateVec.back();

	// 현재 년/월/일
	int nowYear = startDate.Year;
	int nowMonth = startDate.Month;
	int nowDay = startDate.Day;

	/// write File
	std::ofstream writeFile("../Data/Converted/ConvertedSVNCommitData.txt");
	// 시작 날짜와 끝 날짜부터 쓴다.
	std::string seDate = MakeDateStr(startDate);
	writeFile << seDate << '\n';

	seDate = MakeDateStr(endDate);
	writeFile << seDate << '\n';

	int i = 0;
	bool isEnd = false;
	int y = nowYear;
	// 일, 월, 년을 하나씩 증가시켜가며 저장한다.
	// 0이면 그 날짜에 내가 커밋을 안한 것이고,
	// 2022-03-15 이런 식이면 내가 그 날짜에 커밋한 것이다.
	for (int m = nowMonth; m <= 12; m++)
	{
		for (int d = nowDay; d <= 31; d++)
		{
			// 현재 달의 마지막 일자에 도달했을 때
			if (d > g_MonthEndDayTable[m - 1])
			{
				nowDay = 1;
				break;
			}

			// 현재 비교 중인 일자
			Date nowDate = dateVec[i];

			if (nowDate.Day > d || nowDate.Month > m || nowDate.Year > y)
			{
				writeFile << '0' << '\n';
			}
			else
			{
				std::string str = MakeDateStr(dateVec[i]);
				writeFile << str << '\n';

				i++;

				if (i >= dateVec.size())
				{
					isEnd = true;
					break;
				}
			}
		}

		if (isEnd)
			break;

		// 12월이 지나면 다시 1월부터, 년도 증가
		if (m >= 12)
		{
			m = 0;
			y++;
		}
	}

	std::cout << "\n== Write Complete ==\n";
	std::cout << "=== Thank you! ===\n";

	writeFile.close();

	system("pause");
}