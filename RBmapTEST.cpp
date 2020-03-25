#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../LAB1/TreeRB.cpp"
#include <vector>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RBmapTEST
{
	TEST_CLASS(RBmapTEST)
	{
	public:
		Assert assert;
		TEST_METHOD(insert_)
		{
			RBTreeMap<string, int> map;
			string key = "cat";
			int val = 15;
			map.Insert(key, val);
			assert.IsTrue(map.Find(key));
		}
		TEST_METHOD(Get_keys_)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			vector<string> keys = map.GetKeys();
			vector<string> key{ "adp","cat","dog" };
			for (int i = 0; i < key.size(); i++)
				assert.AreEqual(keys[i], key[i]);
		}
		TEST_METHOD(Get_values)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			vector<int> keys = map.GetValues();
			vector<int> key{ 25,10,20 };
			for (int i = 0; i < key.size(); i++)
				assert.AreEqual(keys[i], key[i]);
		}
		TEST_METHOD(find_)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			assert.IsTrue(map.Find("dog"));
		}
		TEST_METHOD(remove_)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			map.Remove("dog");
			assert.IsFalse(map.Find("dog"));
		}
		TEST_METHOD(clear_)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			map.Clear();
			assert.IsFalse(map.Find("dog"));
			assert.IsFalse(map.Find("cat"));
			assert.IsFalse(map.Find("adp"));
		}
		TEST_METHOD(Get_values_empty)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			map.Clear();
			try
			{
				map.GetValues();
			}
			catch (out_of_range error)
			{
				assert.AreEqual(error.what(), "invalid argument");
			}
		}
		TEST_METHOD(Get_keys_empty)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			map.Clear();
			try
			{
				map.GetKeys();
			}
			catch (out_of_range error)
			{
				assert.AreEqual(error.what(), "invalid argument");
			}
		}
		TEST_METHOD(remove_empty)
		{
			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			map.Clear();
			try
			{
				map.Remove("adp");
			}
			catch (out_of_range error)
			{
				assert.AreEqual(error.what(), "invalid argument");
			}
		}
		
		TEST_METHOD(clear_empty)
		{
			RBTreeMap<string, int> map;

			try
			{
				map.Clear();
			}
			catch (out_of_range error)
			{
				assert.AreEqual(error.what(), "Error!Out of range!");
			}
		}
		TEST_METHOD(find_empty)
		{

			RBTreeMap<string, int> map;
			map.Insert("dog", 20);
			map.Insert("cat", 10);
			map.Insert("adp", 25);
			try
			{
				map.Find("gd");
			}
			catch (out_of_range error)
			{
				assert.AreEqual(error.what(), "invalid argument");
			}
		}

	};
}
