//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
//   This file contains the definition of classes 
//   with and the prototypes of the functions.
//
//************************************************************************
#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>

using namespace std;

const int K_SizeMax = 100;

enum Bool{TRUE = 1, FALSE = 0};

//******************************************************************
// Definition of the class Transaction
//******************************************************************
class Transaction
{public:
     Transaction()
     {
        accountId = type = date = code = 0;
        amount = 0.0;
     }
     Transaction(long TrId, int TrType, long TrDate, int TrCode, double TrAmount = 0.0);
     ~Transaction(){}

     // Accessing to the fields of an instance
     long getNumAccount() const {return accountId;}
     int getType() const {return type;}
     long getDate() const {return date;}
     int getCode() const {return code;}
     double getAmount() const {return amount;}

     // Modifying the fields of an instance
     void setAccountId(long accountTr);
     void setType(int typeTr);
     void setDate(long dateTr);
     void setCode(int codeTr);
     void setAmount(double amountTr);
private:
     long accountId, date;
     long type, code;
     double amount;
};

//******************************************************************
// Definition of the class BankAccount
//******************************************************************
class BankAccount
{public:
    inline BankAccount();
    inline BankAccount(long id, int newType, char * name, long date,
                   double newBalance = 0.0);
    inline ~BankAccount();

     // Type of an account
     Bool isBankAccount() const {return TRUE;}
     Bool isCheckingAccount() const {
		 if(type == 01) 
			 return TRUE;
		else
			false;
	 }
     Bool isSavingsAccount() const {
		 if(type == 02) 
			 return TRUE;
		 else
			 false;
	 }

     virtual Bool isDepositAccount() const {return FALSE;}
     virtual Bool isLoanAccount() const {return FALSE;}

     // Possible operations
     virtual Bool acceptWithdrawal() const {return TRUE;}
	 
	// Purpose: This function validates whether the transaction code 
	//          corresponds to the correct account:
	//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
	//              - 02 ==> account (01: Check, 02: Savings)
	//              - 03 ==> account (01: Check).
	Bool validateTransaction(const Transaction trans) const; //
	
	// Purpose: This function is used to execute the transaction already performed 
	// (update the balance of an account).
    virtual void executeTransaction(const Transaction trans);
	
	virtual void print();
	
    Bool operator<(BankAccount c) const {
        return ((c.accountId < accountId) && (c.type < type)) ? TRUE : FALSE; 
     }

     // Accessing to the fields of an instance
     long getAccountId() const {return accountId;}
     int getType() const {return type;}
     char * getClientName() const {return clientName;}
     long getUpdatedate() const {return updateDate;}
     double getBalance() const {return balance;}

     // Modifying the fields of an instance
     void setAccountId(long account);
     void setType(int newType);
     void setClientName(char * name);
     void setUpdateDate(long date);
     void setBalance(double newBalance);

private:
     long accountId;
	 long type;
     char * clientName;
     long updateDate;
     double balance;
};

//******************************************************************
// Definition of the class DepositAccount
//******************************************************************
class DepositAccount : public BankAccount
{public:
    DepositAccount(){nbyears = 0;}
    DepositAccount(long id, int newType, char * name, long date,
                     double balanceArg = 0.0, int nbyear = 1);
    ~DepositAccount(){}
    Bool isDepositAccount() const {return TRUE;}
    Bool acceptWithdrawal() const {return TRUE;}

    // Possible Operations 
    void print();

    // Accessing to the fields of an instance
    int getNbYears() const {return nbyears;}
    double getRate() const {return rate;}

    // Modifying the fields of an instance
    void setNbYears(int nbyear);
    void Bonus();
     
private:
     static double rate;
     int nbyears;
};

//******************************************************************
// Definition of the class LoanAccount
//******************************************************************
class LoanAccount : public BankAccount
{public:
     LoanAccount()
     {
          nbyears = 0;
          rate = 0.0;
     }
     LoanAccount(long id, int newType, char * name, long date,
                    double balanceArg = 0.0, int nbyear = 1, double rt = 8.5);
     ~LoanAccount(){}

     // Possible operations 
     Bool isLoanAccount() const {return TRUE;}
     Bool acceptWithdrawal() const {return FALSE;}
	 
	// Purpose: This function is used to execute the transaction already performed 
	// (update the balance of an account).
     void executeTransaction(const Transaction trans);
     void print();

     // Accessing to the fields of an instance
     int getNbYears() const {return nbyears;}
     double getRate() const {return rate;}

     // Modifying the fields of an instance
     void setNbYears(int nbyear);
     void setRate(double rt);
private:
     int nbyears;
     double rate;
};

//******************************************************************
// Functions to define and to use
//******************************************************************
BankAccount ** readAccounts();
void sortAccounts(BankAccount **);
void updateAccounts(BankAccount **);
void displayAccounts(BankAccount **);
