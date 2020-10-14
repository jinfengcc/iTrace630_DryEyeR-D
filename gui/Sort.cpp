//***************************************************************************************

#include "StdAfx.h"
#include "Sort.h"

//***************************************************************************************
//***************************************************************************************
//***************************************************************************************
//***************************************************************************************

// Bubble sort
/*
void sort(int *a, const int n)
{
for (int i = 0; i < n - 1; i++) {
for (int j = n - 1; j > i; j--) {
if (a[j - 1] > a[j]) Swap(a[j], a[j - 1]);
}
}
}
*/
//***************************************************************************************
// Reversed bubble sort
/*
void sort(int *a, const int n)
{
for (int i = n - 1; i > 0; i--) {
for (int j = 0; j < i; j++) {
if (a[j] > a[j + 1]) Swap(a[j], a[j + 1]);
}
}
}
*/
//***************************************************************************************
// Selection sort
// ��������������� ��������� �� i ����� ��������� ����������� ������.
// ��� ���������� ������ �������� ������ (���������� ����������), ���� �������
// ������������ �� ����� ����������� ������ � ����� ������ ����������� �� ����������.
// ��������������� ��������� �� i ����� ������ �������� ���������� i ����� �������.
/*
void sort(int *a, const int n)
{
for (int i = 0; i < n - 1; i++) {
for (int j = i + 1; j < n; j++) {
if (a[i] > a[j]) Swap(a[j], a[i]);
}
}
}
*/
//***************************************************************************************
// Insertion sort
// ��������������� ��������� ��������� ����������� ������.
// ������ ��� � ��� ��������������� ���������
// ����������� ����� �������, �������� ����������� ��� �����.
// ��������������� ��������� �� i ����� �� �������� ���������� ����� �������, ����
// ����� ������ �� �������� ������� ���������� ������ ���������.
/*
void sort(int *a, const int n)
{
for (int i = 1; i < n; i++) {
for (int j = 0; j < i; j++) {
if (a[j] > a[i]) Swap(a[j], a[i]);
}
}
}
*/
//***************************************************************************************
// Merge sort

//***************************************************************************************
