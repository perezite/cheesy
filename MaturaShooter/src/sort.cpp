#include "objekt_basis.h"

//=========G L O B A L S=========================================================================
list<OBJEKT>::iterator iter1;
list<LPOBJEKT>::iterator iter2 = PointerList.begin();
//=========P R O T O S===========================================================================
void QuickSort(int iLow, int iHigh);
void SwapElems(int iIndex1, int iIndex2);
void GetLength(void);
LPOBJEKT GetElem(int iIndex);
void ClearPointerList(void);
//==========F U N K T I O N E N==================================================================
//-----------------------------------------------------------------------------------------------
void QuickSort(int iLow, int iHigh)
{
	int iTop = iLow;
	int iBottom = iHigh - 1;
	int iPartitionIndex;
	int iPartitionValue;

	// Nur weitermachen, falls Array-Bereich überhaupt korrekt
	if (iLow > iHigh)
		return;

	//=====Vergleich, falls nur (noch) 2 Elemente=====
	// Falls nur noch zwei Elemente --> ggf. vertauschen und Funktion beenden
	if (iHigh == (iLow + 1))
	{
		if(GetElem(iLow)->y > GetElem(iHigh)->y)
		{
			SwapElems(iHigh, iLow);
		}
		return;
	}
	//=====Vergleich, falls mehr als 2 Elemente====
	// Partition-Elem auswählen und an letzte Array-Position setzen
	iPartitionIndex = int((iLow + iHigh)/2);
	iPartitionValue = GetElem(iPartitionIndex)->y;
	SwapElems(iHigh, iPartitionIndex);
	// Eigentlicher Vergleich
	do{
		// Top Index hochzählen, bis Element gefunden, dass grösser/gleich (als) Partition Element
		while((iTop <= iBottom) && ((GetElem(iTop)->y) <= iPartitionValue))
		{
			iTop++;
		}
		// Bottom Index runterzählen, bis "" "", "" kleiner/gleich "" "" ""
		while((iTop <= iBottom) && ((GetElem(iBottom)->y) >= iPartitionValue))
		{
			iBottom--;
		}
		// Falls Indizies nicht auf selbem Element und iTop < iBottom --> vertauschen
		if (iTop < iBottom)
		{
			SwapElems(iTop, iBottom);
		}
	} while (iTop < iBottom);

	//Partition Element an die finale Position setzen
	SwapElems(iTop, iHigh);

	// Rekursiver Aufruf für die Array-Teile links und rechts des Partition Elements
	QuickSort(iLow, iTop - 1);
	QuickSort(iTop + 1, iHigh);
}

//------------------------------------------------------------------------------------------------
void SwapElems(int iIndex1, int iIndex2)
{
	//====Falls Indizies identisch --> abbrechen====
	if (iIndex1 == iIndex2)
	{
		return;
	}
	//====Sonst Zeiger vertauschen====
	else
	{
		list<LPOBJEKT>::iterator iter1;
		list<LPOBJEKT>::iterator iter2;
		//---iter1 auf iIndex1 setzen---
		iter1 = PointerList.begin();
		for (int i = 0;i < iIndex1; i++)
		{
			iter1++;
		}
		// Zeiger des ersten Index temporär speichern
		LPOBJEKT Temp = (*iter1);

		//---iter2 auf iIndex2 setzen---
		iter2 = PointerList.begin();
		for (i = 0;i < iIndex2; i++)
		{
			iter2++;
		}
		// Eigentliches Vertauschen
		(*iter1) = (*iter2);
		(*iter2) = Temp;

	}
}
//---------------------------------------------------------------------------------------------------
LPOBJEKT GetElem(int iIndex)
{
	list<LPOBJEKT>::iterator iter;
	iter = PointerList.begin();
	// Iterator auf iIndex setzen
	for (int i = 0;i < iIndex;i++)
		iter++;
	return (*iter);	
}
//---------------------------------------------------------------------------------------------------
void GetSortedPointerList(void)
{
	// Falls die PointerList nicht leer ist, wird sie erst geleert
	if (!PointerList.empty())
	{
		PointerList.clear();
	}
	// Zeiger von UnitList in PointerList speichern
	iter2 = PointerList.begin();
	for(iter1 = UnitList.begin(); iter1 != UnitList.end(); iter1++)
	{
		PointerList.push_back(&(*iter1));
		iter2++;
	}
	// PointerList sortieren
	QuickSort(0, PointerList.size()-1);
}