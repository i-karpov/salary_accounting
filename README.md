# Salary Accounting Description


# Salary Accounting REST API

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
* *YEAR_SALARY_REPORT*
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