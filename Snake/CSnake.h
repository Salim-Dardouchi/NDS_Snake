/*
 * CSnake.h
 *
 *  Created on: 13 sept. 2021
 *      Author: eleve
 */
#include <nds.h>
#include <nf_lib.h>

#include "../Headers/parameters.h"
enum e_direction{
	DIR_NONE = 0,
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
};

class CSnake {
private:
	class CSection{
	public:
		CSection*		m_pPrev;
		CSection*		m_pNext;
		unsigned int 	m_uDirection;
		int				m_iPosX, m_iPosY;
	public:
		CSection(CSection* pPrev, CSection* pNext, unsigned int uDirection, int iPosX, int iPosY);
		~CSection();
	};
private:
	CSection*		m_pHead;
	CSection*		m_pTail;
	int				m_uNbSection;
	unsigned int	m_uNbSprites;
public:
	CSnake();
	~CSnake();
	void Draw();
	void Move();
	int  ChangeDirection(unsigned int uDirection);
	void GrowUp();
	void Shrink();
	int	 IsOnCell(int iPosX, int iPosY);
	int  IsFoodDetected(int iPosX, int iPosY);
	int	 IsBiteHimself();
	void SetNbSprites(unsigned int uNbSprites);
	int  GetSize();
};
