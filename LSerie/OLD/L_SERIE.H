// l_serie.h: interface for the l_serie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_L_SERIE_H__F20AD7C3_FE91_11D4_ADA9_0050BAA2C3F9__INCLUDED_)
#define AFX_L_SERIE_H__F20AD7C3_FE91_11D4_ADA9_0050BAA2C3F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class l_serie  
{
public:
	l_serie(int port);							//Construi un objet utilisant le port COM "port".	
	l_serie();									//Construi un objet utilisant le port COM1.
	virtual ~l_serie();

	bool  InitComPort();			            //Ouvre le port s�rie
	void  CloseCom();			                //Ferme le port s�rie
	bool  SetTimeout(DWORD ms);					//Fixe le TimeOut
	bool  SetBaud(DWORD baudrate);				//Choix de la vitesse de transmition

	int  SendData(DWORD lg, LPBYTE data);		//Envois une chaine data de lg �l�ments
	int  ReceiveData(DWORD lg, LPBYTE data);	//Recois une chaine data de lg �l�ments

	bool SetRts(int val);						//Positionne RTS au niveau val (0 ou 1)
	bool SetDtr(int val);
	bool SetTxd(int val);
	bool  GetCts();								//Renvois l'�tat de CTS (0 ou 1)
	bool  GetDtr();
	bool  GetRi();
	bool  GetCd();
	

private:
	HANDLE			hcom;				//Fichier de sortie sur le port COM		| The file stream use for acces to the serial port.
	_COMMTIMEOUTS	ct;                 //={0,0,0,0,0}; //Config du Time Out	| This variable contain the delay of the time-out. 
	DCB				dcb;				//Config du Port						| This object is use in order to configure the serial port.
	int	ComPort;						//N� du port s�rie utilis�

};

#endif // !defined(AFX_L_SERIE_H__F20AD7C3_FE91_11D4_ADA9_0050BAA2C3F9__INCLUDED_)
