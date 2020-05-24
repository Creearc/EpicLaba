#include <stdio.h>
#include <string>
#include <iostream>
#include <tuple>

using namespace std;

#define n 1000
#define n2 5
#define n3 1000

// ____________________________________________________________________________________________

// Функция бинарного поиска
int binary(int arr[], int x)
{
  int mn = 0;
  int mx = n;
  int i = n / 2;
  while(x != arr[i])
  {
    if( x > arr[i])
    {
      mn = i;
      i = (i + mx) / 2;
    }
    else
    {
      mx = i;
      i = (mn + i) / 2;
    }
  }
  return i - 1;
}

// ____________________________________________________________________________________________

// Функция вычисления хеша
int myhash(string str)
{
	int out = 0;
	for(int i = 0; i < str.length(); i++)
		out = out + str[i] - 0x30;
	return out % (n2); 
}

// Функция поиска по хешу
float find_by_hash(string str, float prices[], string names[])
{
	int k = myhash(str) * 10;
	for(int j=0; j<10; j++)
	{
		if(names[k + j] == str)
		{
			return prices[k + j];
		}
	}
}

// ____________________________________________________________________________________________


// Узнать с какой стороны расположен потомок относительно родителя
int parent_pos(int ind, int tree[][4])
{
	int pos;
	if(tree[tree[ind][3]][1] == ind)
		pos = 1;
	else
		pos = 2;
	return pos;
}

// Высота поддерева
int tree_h(int ind, int tree[][4], int h=0)
{
	if(ind == 0)
		return 0;
	int h1 = 0; 
	int h2 = 0;
	if(tree[ind][1] != 0)
	{
		h1 = tree_h(tree[ind][1], tree, h + 1);
	}
	if(tree[ind][2] != 0)
	{
		h2 = tree_h(tree[ind][2], tree, h + 1);
	}
	if(tree[ind][1] == 0 and (tree[ind][2] == 0)) 
		return h;
	if(h1 > h2)
		h = h1;
	else
		h = h2;
	return h;
}


// Балансировка ветвей
void balance(int ind, int tree[][4])
{
	int temp;
	int ind2;
	int balance = tree_h(tree[ind][1], tree) - tree_h(tree[ind][2], tree);	// Вычисление баланса поддеревьев
	// Для осуществления поворотов мы просто переназначаем связи между элементами
	if(balance<-1) // Левый поворот
	{
		ind2 = tree[ind][2];
		temp = tree[ind][0];
		tree[ind][0] = tree[ind2][0];
		tree[ind2][0] = temp;
			
		tree[ind][2] = tree[ind2][2];
		tree[ind2][2] = tree[ind2][1];
		tree[ind2][1] = tree[ind][1];
		tree[ind][1] = ind2;	
	}
	if(balance>1) // Правый поворот
	{
		ind2 = tree[ind][1];
		temp = tree[ind][0];
		tree[ind][0] = tree[ind2][0];
		tree[ind2][0] = temp;
			
		tree[ind][1] = tree[ind2][1];
		tree[ind2][1] = tree[ind2][2];
		tree[ind2][2] = tree[ind][2];
		tree[ind][2] = ind2;
	}
			

}

// Добавления элемента в бинарное дерево
void add_to_tree(int ind, int tree[][4])
{
	int i = 0;
	int pos = 0;
	while (true)
	{
		if(tree[ind][0] > tree[i][0])
		{
			if(tree[i][2] == 0)
			{
				pos = 2;
				break;
			}
			else
			{
				i = tree[i][2];
			}
		}
		else if(tree[ind][0] < tree[i][0])
		{
			if(tree[i][1] == 0)
			{
				pos = 1;
				break;
			}
			else
			{
				i = tree[i][1];
			}
		}
		else 
		{
			printf("Element %d exists\n", tree[ind][0]);
			break;
		}

	}
	tree[i][pos] = ind;
	tree[ind][3] = i;
	
	// Балансировка каждого родителя добавленного элемента
	while (i > 0)
	{
		i--;
		balance(i + 1, tree);
		
	}	
	
}

// Поиск числа в бинарном дереве
int find_in_tree(int num, int tree[][4])
{
	int i = 0;
	int pos = 0;
	printf("%d | ", num);
	while (true)
	{
		printf("%d", tree[i][0]);
		if(num > tree[i][0])
		{
			if(tree[i][2] == 0)
			{
				printf(" Doesn't exist\n");
				break;
			}
			else
			{
				i = tree[i][2];
			}
		}
		else if(num < tree[i][0])
		{
			if(tree[i][1] == 0)
			{
				printf(" Doesn't exist\n");
				break;
			}
			else
			{
				i = tree[i][1];
			}
		}
		else 
		{
			printf("\n");
			break;
		}
		printf("->");
	}
	return i;
}

// Удаление числа из бинарном дереве
void delet_from_tree(int num, int tree[][4])
{
	int ind = find_in_tree(num, tree);
	int pos = parent_pos(ind, tree);
	
	if(tree[tree[ind][3]][1] == ind)
	{
		pos = 1;
	}
	else
	{
		pos = 2;
	}
	if(tree[ind][1] !=0 and tree[ind][2] != 0)
	{
		tree[tree[ind][3]][pos] = 0;
		add_to_tree(tree[ind][1], tree);
		add_to_tree(tree[ind][2], tree);
	}
	else if(tree[ind][1] !=0)
	{
		tree[tree[ind][3]][pos] = tree[ind][1];
	}
	else if(tree[ind][2] != 0)
	{
		tree[tree[ind][3]][pos] = tree[ind][2];
	}
	else
	{
		tree[tree[ind][3]][pos] = 0;
	}
}


// ____________________________________________________________________________________________

int main() {   
  
  // Бинарный поиск
  // Сгенерируем массив с элементами от 0 до 999
  printf("\n--------- Binary search ---------\n");
  int arr[n];
  for(int i=0; i<n; i++)
  {
    arr[i] = i;
  }
  // Найдем индекс числа 10
  int x = 10;
  printf("%d\n", binary(arr, x));
  
 // ____________________________________________________________________________________________ 
  
  // Хеширование
  printf("\n--------- Search by hashing ---------\n");
  // Создадим массив продуктов
  string products[n2] = {"apple", "orange", "milk", "croissant", "tomato"};
  // Создадим массивы с учетом возможности коллизий воспользуемся методом открытой адресации
  float prices[n2 * 10]; // для избежания коллизий таблица хешей больше исходного массива 
  string names[n2 * 10]; // для того, чтобы при коллизии убедиться, что мы нашли нужный товар вводим доп. массив
  
  // Сгенерируем массив цен и запишем их в массив
  printf("Prices:\n");
  int k;
  for(int i=0; i<n2; i++)
  {
	k = myhash(products[i]) * 10;
	for(int j=0; j<10; j++)
	{
		if(prices[k + j] == 0)
		{
			prices[k + j] = (i + 1) * 10.5;
			names[k + j] = products[i];
			printf("%s: %frub    hash:%d\n", names[k + j].c_str(), prices[k + j], k + j);
			break;
		}
	}
  }
  
  // Найдем цену молока используя хеш
  string s = {"milk"};
  printf("\nPrice of %s:", s.c_str());
  printf("%f\n", find_by_hash(s, prices, names));
  
  // Найдем цену помидора используя хеш
  s = "tomato";
  printf("\nPrice of %s:", s.c_str());
  printf("%f\n", find_by_hash(s, prices, names));
  
  // ____________________________________________________________________________________________
  
  // Бинарное дерево
  printf("\n--------- Binary tree ---------\n");
  int tree[n3][4]; // Представим дерево в виде двухмерного массива, в котором элемент 
					 //под i индексо содержит значение элемента и ссылки на следующие за ним элементы
					 
  int count = 100; // Количество элементов дерева
  for(int i=0; i<count; i++)
  {
	  tree[i][0] = i; tree[i][1] = 0; tree[i][2] = 0;
  }
  // Перемешаем массив
  int temp;
  int ind;
  int pos;
  for(int i=0; i<count; i++)
  {
	  ind = rand() % count;
	  temp = tree[i][0];
	  tree[i][0] = tree[ind][0]; 
	  tree[ind][0] = temp;
  }
  // Добавляем жлементы в бинарное дерево
  for(int i=1; i<count; i++)
  {  
	add_to_tree(i, tree);

  }
  // Найдем число 67
  printf("Searching for 67:\n");
  find_in_tree(67, tree);
  
  // Найдем число 567 которого изначально нет в дереве
  printf("Searching for 567:\n");
  find_in_tree(567, tree);
  // Добавим число 567
  count++; // Увеличим количество элементов дерева
  tree[count - 1][0] = 567;
  add_to_tree(count - 1, tree);
  printf("567 was added\n");
  // Найдем число 567
  printf("Searching for 567:\n");
  find_in_tree(567, tree);
  
  // Удалим число 93
  printf("Deleting 93:\n");
  count--;
  delet_from_tree(93, tree);
  
  // Поиск чисел от 0 до 100 в дереве
  // Элементы 93 и 100 не будут найдены, так как их нет в дереве
  for(int i=0; i<100; i++)
  {
	  find_in_tree(i, tree);
  }
  
  
 return 0; 
}
