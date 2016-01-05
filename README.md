# Useful commands

sudo service nginx restart

g++ SA_Server.cpp -pthread -lmongoclient -lboost_thread -lboost_system -lboost_regex  -O2 -fPIC -lfastcgi-daemon2 -shared -o lib_SA_Server.so


fastcgi-daemon2 --config=SA_Server.conf


# Salary Accounting - CAP Overview

Система будет иметь модель **CP**. 
Для реализации выбрана база данных MongoDB.

**Consistency**
Система отвечает за учёт денег и построение отчётов о деньгах. На основании данных, предоставляемых системой, инициируются операции над реальными деньгами -- выплаты, премии и т.д. В большинстве ситуаций крайне важно не допускать ошибок в расчётах денег.
Поэтому согласованность данных естественным образом является необходимым требованием к системе.

**Availability**
Операции по учёту выплат сотрудникам обычно делаются в фоновом режиме, и недоступность таких операций не блокирует основную деятельность компаний.
Поэтому доступностью системы можно пожертвовать.

**Partition Tolerance**
Предполагается, что систему могут устанавливать в разных компаниях, с разным качеством IT-инфраструктуры. Поэтому вероятность возникновения обрыва связи между узлами, в общем случае, достаточно велика.
Таким образом, система должна быть устойчива к расщеплению.

**MongoDB**
MongoDB по умолчанию обеспечивает CP и является достаточно распространённой.


# Salary Accounting - REST API


## Data Schema


There are following data types:

* *SALARY_PAYMENT*:
    
```
    
{
        
	"Id": "GUID",

    "PayedToUserId": 123,

    "PayedDate": "YYYY-MM-DD HH:MM:SS",

    "PayedPeriodStartDate": "YYYY-MM-DD HH:MM:SS",

    "PayedPeriodEndDate": "YYYY-MM-DD HH:MM:SS",

    "Amount": 29999
    
}
    
```

* *YEAR_SALARY_REPORT*:

```
    
{
        
	"UserId": 123,

    "YearRank": 17,

    "PayedInMonth1": 1000099,

    "PayedInMonth2": 800099,

    ...
        
	"PayedInMonth12": 130099
    
}
    
```


## Roles

2 roles exist in the system: ADMIN and CLIENT

## Urls

### Create Payment Record
* **Available for roles:** ADMIN
* **Request**
	* **URL:** `/payments`
	* **Method:** `POST`
	* **Body:** *SALARY_PAYMENT*
* **Response**
	* **Code:** `201 HTTP_CREATED`
	* **Body:** Id of created record

### Get Payment by Id
* **Available for roles:** ADMIN, CLIENT
* **REQUEST**
    * **URL:** `/payments/{id}`
    * **Method:** `GET`
* **RESPONSE**
    * **Content-Type:** `application/json`
    * **Code:** `200 HTTP_OK`, `404 HTTP_NOT_FOUND`, `403 HTTP_FORBIDDEN`
    * **Body:** *SALARY_PAYMENT*

### Get user's Payments
* **Available for roles:** ADMIN, CLIENT
* **REQUEST**
    * **URL:** `/users/{id}/payments`
    * **Method:** `GET`
* **RESPONSE**
    * **Content-Type:** `application/json`
    * **Code:** `200 HTTP_OK`, `404 HTTP_NOT_FOUND`, `403 HTTP_FORBIDDEN`
    * **Body:** array of *SALARY_PAYMENT*

### Get user's Payments Report for specific Year
* **Available for roles:** ADMIN, CLIENT
* **REQUEST**
    * **URL:** `/users/{id}/payments/reports/year/{year}`
    * **Method:** `GET`
* **RESPONSE**
    * **Content-Type:** `application/json`
    * **Code:** `200 HTTP_OK`, `404 HTTP_NOT_FOUND`, `403 HTTP_FORBIDDEN`
    * **Body:** *YEAR_SALARY_REPORT*
