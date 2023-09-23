#include <iostream>
#include "windows.h"
#include <vector>
using namespace std;
#pragma warning (disable:4996)

void Color(int color) // ������� ������ �����������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
}

void Message(int color, string text)
{
	Color(color);
	cout << text;
	Color(7);
}

class AbstractVocabulary // ������ ��� ������� ������������ ��������
{
protected:
	vector<string> words;
	int size;
	int maxwordlength;
public:
	virtual string GetWord(int num) = 0;
	virtual int GetSize() = 0;
	virtual int GetMaxWordLength() = 0;
	virtual void Show() = 0;

	~AbstractVocabulary()
	{}
};

class Vocabulary : public AbstractVocabulary// ���������� �������
{
private:

public:
	Vocabulary()
	{
		/*������ ���*/
		words = { "�������������", "�����", "�����������", "�������", "�����", "����", "�����"};
		/* ����� ���������� ���� */
		size = words.size();

		/*�������� �����*/
		maxwordlength = 0;
		for (int i = 0; i < words.size(); i++)
			if (words[i].length() > maxwordlength) maxwordlength = words[i].length();
	}

	string GetWord(int num) override
	{
		return words[num];

	}
	int GetSize() override
	{
		return size;
	}
	int GetMaxWordLength() override
	{
		return maxwordlength;
	}

	void Show() override // ������� ������� - ��������� ������ �������� �� �����
	{
		for (int i = 0; i < words.size(); i++)
			cout << words[i] << endl;
	}
};

class ExternalVocabulary : public AbstractVocabulary// ��������� ������� 
{
private:
	FILE* in;
	char buff[100];
public:

	ExternalVocabulary()
	{
		try
		{
			in = fopen("vocabulary1251.txt", "r");
			if (!in) throw "�� ������� ����������� ���� ��������\n";
			while (fgets(buff, sizeof(buff), in))
			{
				int length = strlen(buff);
				if (length > 0 && buff[length - 1] == '\n') //�������� � ������������� ����� � ����
				{
					buff[length - 1] = '\0';
				}
								
				char* token = strtok(buff, "");
				while (token != nullptr)
				{
					words.push_back(token);
					token = strtok(nullptr, "");
				}
			}
			size = words.size();
			/*�������� �����*/
			maxwordlength = 0;
			for (int i = 0; i < words.size(); i++)
				if (words[i].length() > maxwordlength) maxwordlength = words[i].length();
		}
		catch (const char* str) {
			cout << str;
			throw -1;
		}
	}

	string GetWord(int num) override
	{
		return words[num];

	}
	int GetSize() override
	{
		return size;
	}
	int GetMaxWordLength() override
	{
		return maxwordlength;
	}
	void Show() override // �������
	{
		for (int i = 0; i < size; i++)
		{
			cout << words[i] << endl;
		}
	}
};


class WordLength // ������������ ������� �����
{
private:
	int length;

public:
	WordLength(int max)
	{
		int temp;
		cout << "������ ��������� ������� �����. ����������� ��������: " << max << endl;
		do
		{
			cin >> temp;
			if (temp > max)  cout << "�������� �� ���� ������������ ����������� ��������. ��������� �� ���.\n";
		} while (temp > max);
		length = temp;
	}

	int GetLength()
	{
		return length;
	}
};

class AttemptsNumber // ������������ ������� �����
{
private:
	int attempts;

public:
	AttemptsNumber()
	{
		int tempvalue;
		string temp;
		cout << "������� �����: 1 - 6 �����, 2 - 8 �����" << endl;
		do
		{
			cin >> temp;
			if (temp == "1") tempvalue = 6;
			else if (temp == "2") tempvalue = 8;
			else cout << "��������� �� ���. ������� 1 ��� 2\n";
		} while (temp != "1" && temp != "2");
		attempts = tempvalue;
	}
	int GetNumber()
	{
		return attempts;
	}
};

class Choosevocabulary
{
private:
	bool choosevocabulary;
public:
	Choosevocabulary()
	{
		bool temp;
		string choice;
		cout << "������ �������: 1 - ����������, 2 - ���������" << endl;
		do
		{
			cin >> choice;
			if (choice == "1") temp = false;
			else if (choice == "2") temp = true;
			else cout << "��������� �� ���. ������� 1 ��� 2\n";
		} while (choice != "1" && choice != "2");
		choosevocabulary = temp;
	}
	bool GetNumber()
	{
		if (choosevocabulary) cout << "������ ���������� �������\n";
		else cout << "������ ��������� �������\n";
		return choosevocabulary;
	}
};

class Mode // ������������ ���
{
private:
	WordLength* wordlength;
	AttemptsNumber* attemptsnumber;
	Choosevocabulary* choosevocabulary;

public:
	Mode(WordLength* wordlength, AttemptsNumber* attemptsnumber, Choosevocabulary* choosevocabulary)
		: wordlength(wordlength), attemptsnumber(attemptsnumber), choosevocabulary(choosevocabulary) {}

	int GetLength()
	{
		return wordlength->GetLength();
	}
	int GetNumber()
	{
		return attemptsnumber->GetNumber();
	}
	int Getocabulary()
	{
		return choosevocabulary->GetNumber();
	}
};


class Choice // ��������� ���������� ����� 
{
private:
	int choice;
public:
	Choice(AbstractVocabulary *vocabulary) : choice(rand() % vocabulary->GetSize()) {};
	int GetChoice()
	{
		return choice;
	}
};

class Word // ����� � ��������
{
private:
	string word;
	int length;
public:
	Word(string word) : word(word) {
		length = word.length();
	}
	bool Iflength(int length)
	{
		if (this->length >= length) return true;
		else return false;
	}
	Word(Choice num, int wordlength, AbstractVocabulary *vocabulary)
	{
		bool iflength = false;
		int choice = num.GetChoice();
		int temp = choice;
		bool trigger = true;
		do
		{
			Word tempword(vocabulary->GetWord(choice));
			if (tempword.Iflength(wordlength)) { word = tempword.GetWord(); break; }
			else
			{
				if (choice < vocabulary->GetSize() - 1 && trigger) choice++;
				else {
					trigger = false;  temp--; choice = temp;
				};
			}
		} while (true);
		length = word.length();
	};
	string GetWord()
	{
		return word;
	}
	int GetLength()
	{
		return length;
	}
	void Show()
	{
		cout << word << endl;
	}
};

class Letter
{
private:
	char letter;
public:
	void SetLetter()
	{
		char temp;
		Message(11, "������ �����\n");
		cin >> temp;
		letter = temp;
	}

	char GetLetter()
	{
		return letter;
	}
	void Show()
	{
		cout << letter;
	}

};


class Figure
{
private:
	int attempts;
public:
	Figure(int attempts) : attempts(attempts) {};


	void Draw(int attempt)
	{
		switch (attempt)
		{
		case 8:
			Color(12);
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|         |      \n"
				<< "|        / \\      \n"
				<< "|       /    \\     \n"
				<< "|                 \n";
			Color(7);
			break;
		case 7:
			Color(14);
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|         |      \n"
				<< "|        / \\      \n"
				<< "|       /         \n"
				<< "|                 \n";
			Color(7);
			break;
		case 6:
			if (attempts == 6) Color(12);
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|        -|-     \n"
				<< "|        / \\      \n"
				<< "|                 \n";
			Color(7);
			break;
		case 5:
			if (attempts == 6) Color(14);
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|        -|-     \n"
				<< "|        /      \n"
				<< "|                 \n";
			Color(7);
			break;
		case 4:
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|        -|-     \n"
				<< "|                \n"
				<< "|                 \n";
			break;
		case 3:
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|        -|      \n"
				<< "|                \n"
				<< "|                 \n";
			break;
		case 2:
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|         |      \n"
				<< "|                \n"
				<< "|                 \n";
			break;
		case 1:
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|         O      \n"
				<< "|                \n"
				<< "|                \n"
				<< "|                 \n";
			break;
		default:
			cout << "-----------------\n"
				<< "|         |      \n"
				<< "|                \n"
				<< "|                \n"
				<< "|                \n"
				<< "|                 \n";
			break;

		}

	}
};

class WordTable
{
private:
	Word* word;
	Figure* figure;
	Letter letter;
	Mode* mode;
	int length; //������� �����
	bool* ifopen;// �������� �� ������� �����
	int attempts; // ������ ������
	char* falseletters = nullptr;
	bool victory;// ������ �� ���������� ��� (��������)
	bool failure;// ������ �� ���������� ��� (�������)


public:
	WordTable(Word* word, Figure* figure, Mode* mode) : word(word), figure(figure), mode(mode)
	{
		length = word->GetLength();
		ifopen = new bool[length];
		for (int i = 0; i < length; i++)
			ifopen[i] = false; // �� ������� �� ����� ������
		attempts = 0;
		
		victory = false;
		failure = false;
	}

	WordTable(const WordTable& obj)
	{
		length = obj.length;
		attempts = obj.attempts;
		ifopen = new bool[length];
		for (int i = 0; i < length; i++)
		{
			ifopen[i] = obj.ifopen[i];
		}
		falseletters = new char[attempts];
		for (int i = 0; i < attempts; i++)
		{
			falseletters[i] = obj.falseletters[i];
		}
		word = obj.word;
		figure = obj.figure;
		letter = obj.letter;
		mode = obj.mode;
		victory = obj.victory;
		failure = obj.failure;
	}

	bool IfFinish() // ���� ������ ���������
	{
		if (attempts == mode->GetNumber())
		{
			failure = true;
			return true;
		}
		for (int i = 0; i < length; i++)
		{
			if (ifopen[i] != true) return false;
		}
		victory = true; return true;
	}

	void OpenLetter() // ��������� �����
	{
		bool success = false;
		letter.SetLetter();
		for (int i = 0; i < length; i++)
			if (word->GetWord()[i] == letter.GetLetter())
			{
				ifopen[i] = true; success = true;
			}
		if (!success) AddFalseLetter(letter);
		Show();

		if (!IfFinish()) OpenLetter(); // ���� ������ ���������
		else if (victory)
		{
			Message(10, "�� �������!\n");

		}
		else if (failure)
		{
			Message(12, "�� �������!\n");

			word->Show();
		}
	}

	void AddFalseLetter(Letter letter) // ������ �������� ���� 
	{
		char* temp = new char[attempts + 1]; 
		for (int i = 0; i < attempts; i++)
		{
			temp[i] = falseletters[i];
		}
		delete[] falseletters;
		temp[attempts] = letter.GetLetter();
		falseletters = temp;
		attempts++;
	}

	void Show() // �� ������ - �����, �������� ����� � ���������
	{
		system("cls");
		cout << endl;
		Message(11, "�����: ");
		Color(15);
		for (int i = 0; i < length; i++)
			if (!ifopen[i]) cout << "_ ";
			else cout << word->GetWord()[i] << " ";
		cout << endl;
		Message(11, "�������: ");
		Color(12);
		for (int i = 0; i < attempts; i++)
		{
			cout << falseletters[i];
			if (i < attempts - 1) cout << ", ";
			else cout << ".\n";
		}
		Color(7);
		cout << endl;
		figure->Draw(attempts);
	}

	bool GetResult()
	{
		if (victory) return true;
		if (failure) return false;
	}

	~WordTable()
	{
		delete[] ifopen;
		delete[] falseletters;
	}

};

class Timer // ��� ���������
{
private:
	int start;
	int end;
public:
	Timer() : start(clock()) {}
	void TimeSpent()
	{
		end = clock();
		cout << "\n��������� " << float(end - start) / 1000 << " ������";
	}
};

class Music // �� ���������� ��� ��� ������.
{
private:
	WordTable* wordtable;
public:
	Music(WordTable* wordtable) : wordtable(wordtable) {};
	void Play()
	{
		if (wordtable->GetResult())
		{
			Beep(277, 200); Beep(349, 200); Beep(415, 200); Beep(554, 800); 
		}
		else
		{
			Beep(392, 200); Beep(392, 200); Beep(392, 200); Beep(311, 800);
		}
	}
};

AbstractVocabulary* createVocabulary(bool useExternal) {
	if (useExternal) {
		return new ExternalVocabulary;
	}
	else {
		return new Vocabulary;
	}
}



int main()
{
	setlocale(LC_ALL, "ukr");
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);
	srand(time(NULL));

	/*�������*/
	try {
	
	Choosevocabulary choosevocabulary;
	AbstractVocabulary* vocabulary = createVocabulary(choosevocabulary.GetNumber());


	//vocabulary->Show(); system("pause"); //����: �������� �� ����� �����, �������� ����'������, �� ��� ��� �� ������� �� ���� ��������


	/* ����������� ������������ */
	WordLength wordlength(vocabulary->GetMaxWordLength());
	AttemptsNumber attemptsnumber;
	
	Mode gamemode(&wordlength, &attemptsnumber, &choosevocabulary);

	/* ����'���� ������ ����� */
	Choice choice(vocabulary);
	Word word(choice, gamemode.GetLength(), vocabulary);
	Figure figure(gamemode.GetNumber());
	WordTable wordtable(&word, &figure, &gamemode);


	/* ��� */

	wordtable.Show();
	Timer timer;
	wordtable.OpenLetter();
	timer.TimeSpent();
	Music music(&wordtable);
	music.Play();

	delete vocabulary;
}

	catch (int) {};

}
