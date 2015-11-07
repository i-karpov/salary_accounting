# Salary Accounting Description



# Salary Accounting REST API


## Data Schema


## Roles
2 roles exist in the system: ADMIN and CLIENT

## Urls

### Create Payment Record

* **Available for roles:** ADMIN

* **Request**
    
	* **URL:** `/payments`
    
	* **Method:** `POST`
    
	* **Body:** [SalaryPayment]

* **Response**
    
	* **Code:** `201 HTTP_CREATED`
   
	* **Body:** Id of created record