//************************************************************************
//
//   Project
//   Fall 2019
//
//   Mohamed TALEB
//
// This program reads information about clients and transactions in the following 2 files:
//          - clients.txt
//          - transact.txt
//
// It also allows certain operations on the read data:
//          - Display client bank accounts
//          - Updating client accounts
//
//************************************************************************
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <map>
#include <vector>

#include "BankABC.h"

using namespace std;
namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm ;
        stm << n ;
        return stm.str() ;
    }
}

double DepositAccount::rate = 6.50;
double totalAccounts = 0.0;

//******************************************************************
// Basic functions of the BankAccount class
//******************************************************************
inline BankAccount::BankAccount()
{
    accountId = type = updateDate = 0;
    clientName = new char[1];
    balance = 0.0;
}

inline BankAccount::BankAccount(long id, int newType,
                                    char * name, long newDate, double newBalance):
                                    accountId(id), type(newType),
                                    updateDate(newDate), balance(newBalance)
{
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline BankAccount::~BankAccount()
{
     if (clientName)
     {
          delete [] clientName;
     }
}

inline void BankAccount::setAccountId(long account)
{
     accountId = account;
}

inline void BankAccount::setType(int newType)
{
     type = newType;
}

inline void BankAccount::setClientName(char * name)
{
     if (clientName)
     {
          delete [] clientName;
     }
     clientName = new char[strlen(name) + 1];
     strcpy(clientName, name);
}

inline void BankAccount::setUpdateDate(long newDate)
{
     updateDate = newDate;
}

inline void BankAccount::setBalance(double newBalance)
{
     balance = newBalance;
}

void BankAccount::print() 
{
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << accountId << "\t\t\t" << type << "\t" << updateDate << "\t\t" << balance;
}

//******************************************************************
// Basic functions of the DepositAccount class
//******************************************************************
inline DepositAccount::DepositAccount(long id, int newType, char * name,
                                    long newDate, double balanceArg, int nbyear) :
                                    BankAccount(id, newType, name,
                                    newDate, balanceArg), nbyears(nbyear)
{}

inline void DepositAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

void DepositAccount::print() 
{
    Bonus();
    BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Functions of the class DepositAccount. It allows to calculate
// the annual bonus on deposit accounts.
// Inputs: Nothing
// Outputs: Nothing
//******************************************************************
void DepositAccount::Bonus()
{
  setBalance(getBalance() + (getBalance()* getRate() * (getNbYears())/36000.00));
}



//******************************************************************
// Basic functions of the LoanAccount class
//******************************************************************
inline LoanAccount::LoanAccount(long id, int newType, char * name,
                                    long newDate, double newBalance, int nbyear,
                                    double newRate) : BankAccount(id, newType,
                                    name, newDate, newBalance), nbyears(nbyear), rate(newRate)
{ }

inline void LoanAccount::setNbYears(int nbyear)
{
     nbyears = nbyear;
}

inline void LoanAccount::setRate(double newRate)
{
     rate = newRate;
}

void LoanAccount::print() 
{
    BankAccount::print();
    cout.setf(ios::fixed);
    cout.precision(2);
    cout << "\t" << nbyears << "\t\t" << rate << endl;
}

//******************************************************************
// Basic functions of the Transaction class
//******************************************************************
inline Transaction::Transaction(long idTr, int typeTr, long dateTr,
                                    int codeTr = 01, double amountTr):
                                    accountId(idTr), type(typeTr),
                                    date(dateTr), code(codeTr),
                                    amount(amountTr)
{ }

inline void Transaction::setAccountId(long compteTr)
{
     accountId = compteTr;
}

inline void Transaction::setType(int typeTr)
{
     type = typeTr;
}

inline void Transaction::setDate(long dateTr)
{
     date = dateTr;
}

inline void Transaction::setCode(int codeTr)
{
     code = codeTr;
}

inline void Transaction::setAmount(double amountTr)
{
     amount = amountTr;
}




//****************************************************************************
// Purpose: Sort a list of bank accounts in ascending order of ids and types.
//
// WARNING: This sort leaves the last account (of id 0) of the list which has 
// its position to assure the tests of end of list later !!!
//
// Inputs: listAccount(BankAccount *), a list of bank accounts.
// Outputs: listAccount(BankAccount *), sorted list of bank accounts.
//****************************************************************************
void sortAccounts(BankAccount ** list)
{
	int i = 0;
	for (i = 0; i <= K_SizeMax - 1; i++) {
		for (int j = i + 1; j <= K_SizeMax; j++) {
			if (*(list + j) == NULL) {
				break;
			}
			int accId1 = (*(list + i))->getAccountId();
			int accId2 = (*(list + j))->getAccountId();
			accId1 == 0 ? accId1 = 999999: accId1 = (*(list + i))->getAccountId();
			accId2 == 0 ? accId1 = 999999: accId2 = (*(list + j))->getAccountId();
			if (accId1 > accId2)
			{
				BankAccount *temp = *(list + i);
				*(list + i) = *(list + j);
				*(list + j) = temp;
			}
		}
	}
}

//******************************************************************
// Purpose: This function reads the file 'clients.txt' and builds 
// an array containing the different bank accounts of customers.
//
// Inputs: Nothing
// Output: listAccount(type: BankAccount *), the list of bank 
//         accounts read from the file 'clients.txt'.
//******************************************************************
BankAccount ** readAccounts()
{
    ifstream inputFile("CLIENTS.TXT");	// Opening the input file
    if (!inputFile)            		// If the file is not found...
    {
        cout << "File not found !!!" << endl;
        exit(0);
    }

    BankAccount ** listAccounts = new BankAccount*[K_SizeMax];
    if (!listAccounts) {
        cout << "Full memory. !!!" << endl;
        return listAccounts;
    }

    BankAccount ** pAccount = listAccounts;
	 
    long accountRead, dateRead;
    int TypeRead, nbyearRead, counter = 0;
    double balanceRead, RateRead;
    char nameRead[60];
	 
    inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
    inputFile.getline(nameRead, 60);
	 
    while (inputFile && (counter < K_SizeMax - 1)){
        // YOU HAVE TO DO SOMETHING FROM HERE !!!
	
	if (TypeRead == 03 ) {
          *pAccount = new DepositAccount(accountRead, TypeRead, nameRead, dateRead,balanceRead, nbyearRead);    
        } else if (TypeRead == 04 ) {
          balanceRead = balanceRead + ((balanceRead * nbyearRead * RateRead) / 36000);
          *pAccount = new LoanAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead, nbyearRead, RateRead);
        }else {
          *pAccount = new BankAccount(accountRead, TypeRead, nameRead, dateRead, balanceRead);
            
        }

        // UNTIL THIS POINT.

          inputFile >> accountRead >> TypeRead >> dateRead >> balanceRead >> nbyearRead >> RateRead;
          inputFile.getline(nameRead, 60);
          pAccount++;
          counter++;
    }
     *pAccount = new BankAccount();
     return listAccounts;
}





//*****************************************************************************************
// Purpose: This function validates whether the transaction code 
//          corresponds to the correct account:
//              - 01 ==> account (01: Check, 02: Savings, 03: Deposit and 04: Loan)
//              - 02 ==> account (01: Check, 02: Savings)
//              - 03 ==> account (01: Check).
//
// Inputs: trans (Type: Transaction) an instance of the Transaction class.
// Outputs: true (Type bool) if the transaction matches one of the accounts listed above.
// false (Type bool) otherwise.
//*****************************************************************************************
Bool BankAccount::validateTransaction(const Transaction trans) const
{
    if ( ( (trans.getCode() == 02) && ( isDepositAccount() || isLoanAccount()) ) ||
         ( (trans.getCode() == 03) && ( isDepositAccount() || isLoanAccount() || isSavingsAccount() ) ) )
      {
       return FALSE;
      }
    else
      {
       return TRUE;
      }
        
}





//******************************************************************************
// Purpose: This function is used to execute the transaction already performed 
// (update the balance of an account).
//
// Inputs: trans (Transaction Type), instance of Transaction class
// Outputs: Nothing
//*******************************************************************************
void BankAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
       {
         if (trans.getCode() == 01)    // Deposit
           {
             setBalance(getBalance() + trans.getAmount());
           }
         else 
           { if (trans.getCode() == 02)    // Withdrawal
                {
                  if (getBalance() >= trans.getAmount())
                     { setBalance(getBalance() - (trans.getAmount() + 0.50)); }
                  else {cout << " insufficient balance!! " << endl; }
                }
             else 			// Check
                {
                  if (getBalance() >= trans.getAmount())
                     { 
                       setBalance(getBalance() - trans.getAmount());
                     }
                  else {cout << " insufficient balance!! " << endl; }
                }
           }
                 
       }   
 
}

//***********************************************************************
// Purpose: This function is used to execute a read transaction
// (updating the balance of the loan account).
//
// Inputs: trans (Transaction Type), instance of the Transaction class
// Outputs: Nothing
//***********************************************************************
void LoanAccount::executeTransaction(const Transaction trans)
{
     if (validateTransaction(trans))
     {
        if (trans.getCode() == 01)    // Deposit
        {
          setBalance(getBalance() - trans.getAmount());
         
        }
     }
}





//*************************************************************************
// Purpose: This function allows to read the file 'transact.txt' 
//          and to update the accounts concerned by the transactions read.
//
// Inputs: listAccount (type BankAccount *), the list of bank accounts.
// Output: Nothing.
//*************************************************************************
void updateAccounts(BankAccount ** listAccounts) {
    ifstream inputFile("TRANSACT.TXT");	// Opening the input file

    if (!inputFile)            		// If the file is not found...
    {
        cout << "File not found !!!" << endl;
        exit(0);
    }

    long accountID, dateOfTransaction;
    int accountType,transactionCode;
    int counter = 0;
    char name[60];
    double amount;

    inputFile >> accountID>> accountType >> dateOfTransaction >> transactionCode >> amount;
    inputFile.getline(name, 60 );

    while(inputFile && (counter<K_SizeMax-1)){

        //Transaction(long idTr, int typeTr, long dateTr,                            int codeTr = 01, double amountTr)

        Transaction * nextTransaction = new Transaction(accountID, accountType, dateOfTransaction, transactionCode, amount);
        BankAccount *currentAccount;

        int listSize = sizeof(listAccounts);

        for (int i = 0; i< listSize; ++i){
            if(listAccounts[i]->getAccountId() == accountID){
                currentAccount = listAccounts[i];
            }
        }
        currentAccount->executeTransaction(*nextTransaction);
        currentAccount->setUpdateDate(dateOfTransaction);

        inputFile >> accountID >> accountType >> dateOfTransaction >> transactionCode >> amount;
        nextTransaction++;
        counter++;
    }
}

//******************************************************************************
// Purpose: This function displays the list of bank accounts for all customers.
//
// Inputs: listAccount (type: BankAccount *), the list of bank accounts.
// Outputs: Nothing
//******************************************************************************
void displayAccounts(BankAccount ** listAccounts)
{
    cout << endl << endl << endl;
    
    Bool find[K_SizeMax];
    for(int k = 0; k < K_SizeMax; k++) {find[k] = FALSE;}

    cout << "                       THE REPORT OF THE BANK ACCOUNTS OF CLIENTS" << endl;
    cout << "                       ------------------------------------------" << endl << endl;
    int i = 0;
	

    // modify this.. added to check if correct information was added.. you may remove :)
    map<int, vector<string> > result_string_map;
    map<int, vector<double> > total_balance_map;
    for(i = 0; i< K_SizeMax; i++){
      if(listAccounts[i] != 0 && listAccounts[i] -> getAccountId() != 0){
        int nbYears;
        double rate;
        if(listAccounts[i] -> isDepositAccount()){
          DepositAccount downcastDepositAcc;
          DepositAccount* depositAcc = dynamic_cast<DepositAccount*>(listAccounts[i]);
          nbYears = depositAcc -> getNbYears();
          rate = depositAcc -> getRate();
        } else if(listAccounts[i] -> isLoanAccount()){
          LoanAccount downcastLoanAcc;
          LoanAccount* loanAcc = dynamic_cast<LoanAccount*>(listAccounts[i]);
          nbYears = loanAcc -> getNbYears();
          rate = loanAcc -> getRate();
        } else{
          nbYears = NULL;
          rate = NULL;
        }
        string s = "                                                                      ";
        s.insert(0, patch::to_string(listAccounts[i] -> getAccountId()));
        s.insert(15, patch::to_string(listAccounts[i] -> getType()));
        s.insert(23, patch::to_string(listAccounts[i] -> getUpdatedate()));
        s.insert(37, patch::to_string(listAccounts[i] -> getBalance()));
        if(nbYears) { s.insert(49, patch::to_string(nbYears)); };
        if(rate) { s.insert(61, patch::to_string(rate)); };

        vector<double> total_balance_vec = total_balance_map[listAccounts[i] -> getAccountId()];
        total_balance_vec.push_back(listAccounts[i] -> getBalance());
        total_balance_map[listAccounts[i] -> getAccountId()] = total_balance_vec;

        vector<string> result_string_vec = result_string_map[listAccounts[i] -> getAccountId()];
        if(result_string_vec.empty()){ result_string_vec.push_back(listAccounts[i] -> getClientName()); };
        result_string_vec.push_back(s);
        result_string_map[listAccounts[i] -> getAccountId()] = result_string_vec;
      };
    }

    for (auto& t : result_string_map){
      cout << "     Client Name: " << t.second[0] << endl << endl;;
      cout << "Bank Account   Type    Update Date   Balance     Nb. Years   Rate" << endl;
      cout << "------------   ----    -----------   --------    ---------   ----" << endl ;
      for (int i = 1; i < t.second.size(); i++){
        cout << t.second[i] << "\n";
      }
      double total_balance = 0;
      for (int i = 0; i < total_balance_map[t.first].size(); i++){
        total_balance = total_balance + total_balance_map[t.first][i];
      }
      cout << "                                     --------" << endl;
      cout << "                     TOTAL ACCOUNTS: " << total_balance << endl;
      cout << endl << endl;
    }

	
}




int main()
{
    BankAccount ** list = readAccounts();
    sortAccounts(list);
    displayAccounts(list);
    updateAccounts(list);
    cout << endl << endl;
    cout << "               ************************************************" << endl;
    cout << "               * REAFFICHAGE DES DONNEES APRES LA MISE A JOUR *" << endl;
    cout << "               ************************************************" << endl;
    displayAccounts(list);
    cout << endl;

	system("PAUSE");
	return 0;
}
