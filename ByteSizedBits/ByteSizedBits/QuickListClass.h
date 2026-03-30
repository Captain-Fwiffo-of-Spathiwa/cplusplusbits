#pragma once

#include <vector>
#include <iostream>
#include <string>

struct SomeStruct
{
    int Number = 1;
    char Letter = 'a';

    std::string ToString()
    {
        return std::to_string(Number) + Letter + ", ";
    }
};


class QuickListClass
{

public: 

    QuickListClass()
    {
        ListOfInts.clear();
        ListOfInts.push_back(1);
        ListOfInts.push_back(2);
        ListOfInts.push_back(3);

        ListOfStructs.clear();
        ListOfStructs.push_back({ 1, 'a' });
        ListOfStructs.push_back({ 2, 'b' });
        ListOfStructs.push_back({ 3, 'c' });
    }
    
    const std::vector<int>& GetListOfInts() const
    {
        return ListOfInts;
    }

    const std::vector<SomeStruct>& GetListOfStructs()
    {
        return ListOfStructs;
    }

    const std::string ToString()
    {
        std::string returnString = "\nInts: ";

        for (int i : ListOfInts)
        {
            returnString += std::to_string(i) + ", ";
        }

        returnString += "\nStructs: ";
        for (auto i : ListOfStructs)
        {
            returnString += i.ToString();
        }

        return returnString;
    }


private:

    std::vector<int> ListOfInts;
    std::vector<SomeStruct> ListOfStructs;
};

/* ------------------------------------------------------------
    From a C# assessment:

    We need the TaskList to provide access to its internal
    list of Tasks. Unless we make a full copy, just returning
    it raw would leave things wide open for the caller to:
        - redirect our list of Tasks to some other list
        - change which Tasks are in our list
        - redirect those Tasks to some other Tasks
        - change what contents are in those Tasks

    Locking everything down in this situation is one of the
    few things C# makes more painful than C++.

    First we need a readonly interface to the specific type
    we're returning. In this case, .NET provides one for the
    List class specifically: IReadOnlyList. Whew!

    That covers the first 3 points above, but the Tasks
    themselves are still naked. To protect them also we would
    need to change some architecture, like using records, or
    init Properies, or more interface setup we'd have to code.

    Meanwhile in C++: Use const, once.

    private:
        vector<Task> MyTasks;
    public:
        const vector<Task>& GetTasks() { return MyTasks; }

    Admittedly in both languages, editing the stuff you
    shouldn't is still only a cast away.

    For this assignment I won't bother with the full cover.
    I'll use IReadOnlyList and just hope the caller gets the
    signalled intent and doesn't muck with the Tasks.
   -----------------------------------------------------------*/