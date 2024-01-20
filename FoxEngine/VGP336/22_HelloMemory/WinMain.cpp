#include<FoxEngine/Inc/FoxEngine.h>

using namespace FoxEngine;
using namespace FoxEngine::Core;

class Student
{
public:

	Student() = default;
	Student(const std::string _name) : mName(_name), mMarks(0) {}

	void SetMark(int _mark) { mMarks = _mark; }

private:
	std::string mName;
	int mMarks;
	int mFinalMarks;

};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	TypedAllocator studentPool = TypedAllocator<Student>("Student POOL", 120);

	Student* a = studentPool.New("st A");
	Student* b = studentPool.New("st B");

	std::vector<Student*> students;

	for (int i = 0; i < 100; i++)
	{
		std::string name = "Student" + std::to_string(i);
		students.push_back(studentPool.New(name));
	}

	for (int i = 0; i < 20; i++)
	{
		auto student = students.back();
		students.pop_back();
		studentPool.Delete(student);
	}

	for (int i = 0; i < 30; i++)
	{
		std::string name = "Student" + std::to_string(students.size() + i);
		students.push_back(studentPool.New(name));
	}


	for (auto Student : students)
	{
		studentPool.Delete(a);
	}

	studentPool.Delete(a);
	studentPool.Delete(b);

	return 0;
}