#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <filesystem>

// ������ �� �Ͼ� �ִ���
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

// �ߺ� üũ
bool CheckOverlap(const Date& d1, const Date& d2)
{
	return ((d1.Year == d2.Year) && (d1.Month == d2.Month) && (d1.Day == d2.Day));
}

// ���� �Լ�
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

// �����ΰ�?
bool isNum(const char& c)
{
	return (c >= '0' && c <= '9');
}

// C++ 17, ������ ��ο� �̸��� �����Ѵ�.
void LoadFilePathAndName_Recursive(std::wstring folderPath, std::unordered_map<std::string, std::string>& map, int& totalNum)
{
	const std::filesystem::path _folderPath{ folderPath.c_str() };
	std::filesystem::create_directories(_folderPath);

	for (const auto& file : std::filesystem::recursive_directory_iterator{ _folderPath })
	{
		std::wstring filePathW = file.path();
		std::string filePath(filePathW.begin(), filePathW.end());
		std::string fileName = filePath.substr(filePath.rfind("/") + 1, filePath.rfind("."));

		// Ȯ���� ����
		fileName = fileName.substr(0, fileName.size() - 4);

		// file path & file name ����
		map[filePath] = fileName;

		totalNum++;
	}
}

// Date�� ��Ʈ������ ������ (2022,3,15 -> 2022-03-15)
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
	std::vector<Date> dateVec;		// ���ڵ��� �� ����
	std::unordered_map<std::string, std::string> filePathAndNameUMap;	// ���� ��ο� �̸�
	int totalNum = 0;	// �� ����
	int count = 0;		// ���� ���� ����

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

		// �� �پ� ������ ����
		while (std::getline(file, str))
		{
			if (std::string::npos == str.find("Date"))
				continue;

			Date date;
			for (int i = 0; i < str.size();)
			{
				char nowChar = str[i];

				// ������ ���
				if (isNum(nowChar))
				{
					// ���ڸ� �Ľ��ϰ�, ������ �Ľ��� ������ Ÿ���� ����
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
						// ���� ���ڵ� ������ ���
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

					// Day���� �Ľ��� �Ǿ��� ��
					// ���Ϳ� �ְ� �ʱ�ȭ
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

	// ������ ������ ����
	sort(dateVec.begin(), dateVec.end(), FuncSort);

	// �ߺ��Ǵ� ��¥ ����
	dateVec.erase(std::unique(dateVec.begin(), dateVec.end(), CheckOverlap), dateVec.end());

	// ���� ��/��/��
	Date startDate = dateVec.front();

	// �� ��/��/��
	Date endDate = dateVec.back();

	// ���� ��/��/��
	int nowYear = startDate.Year;
	int nowMonth = startDate.Month;
	int nowDay = startDate.Day;

	/// write File
	std::ofstream writeFile("../Data/Converted/ConvertedSVNCommitData.txt");
	// ���� ��¥�� �� ��¥���� ����.
	std::string seDate = MakeDateStr(startDate);
	writeFile << seDate << '\n';

	seDate = MakeDateStr(endDate);
	writeFile << seDate << '\n';

	int i = 0;
	bool isEnd = false;
	int y = nowYear;
	// ��, ��, ���� �ϳ��� �������Ѱ��� �����Ѵ�.
	// 0�̸� �� ��¥�� ���� Ŀ���� ���� ���̰�,
	// 2022-03-15 �̷� ���̸� ���� �� ��¥�� Ŀ���� ���̴�.
	for (int m = nowMonth; m <= 12; m++)
	{
		for (int d = nowDay; d <= 31; d++)
		{
			// ���� ���� ������ ���ڿ� �������� ��
			if (d > g_MonthEndDayTable[m - 1])
			{
				nowDay = 1;
				break;
			}

			// ���� �� ���� ����
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

		// 12���� ������ �ٽ� 1������, �⵵ ����
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