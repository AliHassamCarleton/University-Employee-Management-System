-- cuNICSdb setup file
-- Michael van Dyk

-- DROP OLD TABLES IF THEY EXIST

drop table if exists user;
drop table if exists payroll_specialist;
drop table if exists employee;
drop table if exists raise;
drop table if exists role;
drop table if exists deduction;
drop table if exists has_deduction;
drop table if exists leave;

BEGIN TRANSACTION;

--CREATE DATABASE TABLES

create table user (
    user_id TEXT primary key NOT NULL
);

create table payroll_specialist (
    user_id TEXT primary key NOT NULL references user(user_id)
);

create table employee (
    employee_id INTEGER primary key NOT NULL,
    employee_given_name TEXT NOT NULL,
    employee_family_name TEXT NOT NULL,
    employee_address_street TEXT NOT NULL,
    employee_address_postal TEXT NOT NULL,
    employee_address_city TEXT NOT NULL,
    employee_address_province TEXT NOT NULL,
    employee_address_country TEXT NOT NULL,
    user_id TEXT NOT NULL references user(user_id)
);

create table raise (
    raise_date INTEGER NOT NULL,
    role_type INTEGER NOT NULL,
    raise_fixed_or_percent INTEGER NOT NULL,
    raise_amount REAL NOT NULL,
    primary key (raise_date, role_type)
);

create table role (
    role_id INTEGER primary key NOT NULL,
    role_start_date INTEGER NOT NULL,
    role_end_date INTEGER default NULL,
    role_salary REAL NOT NULL,
    role_type INTEGER NOT NULL references raise(role_type),
    employee_id INTEGER NOT NULL references employee(employee_id)
);

create table deduction (
    deduction_id INTEGER primary key NOT NULL,
    deduction_name TEXT NOT NULL,
    deduction_fixed_or_percent INTEGER NOT NULL,
    deduction_amount REAL NOT NULL
);

create table has_deduction (
    role_type INTEGER NOT NULL references role(role_type),
    deduction_id INTEGER NOT NULL references deduction(deduction_id),
    primary key (role_type, deduction_id)
);

create table leave (
    leave_id INTEGER primary key NOT NULL,
    leave_start_date INTEGER NOT NULL,
    leave_end_date INTEGER NOT NULL,
    leave_percentage REAL NOT NULL,
    role_id INTEGER NOT NULL references role(role_id)
);

-- test values, comment out if not testing
--insert into user (user_id) values ('pstest');
--insert into user (user_id) values ('empfemtest');
--insert into user (user_id) values ('empmaltest');

--insert into payroll_specialist (user_id) values ('pstest');

--insert into employee (employee_id, employee_given_name, employee_family_name, employee_address_street, employee_address_postal, employee_address_city, employee_address_province, employee_address_country, user_id) values (123456, 'Jane', 'Doe', '123 Fake Street', 'A1B 2C3', 'Fakeville', 'Faketario', 'Fakeda', 'empfemtest');
--insert into employee (employee_id, employee_given_name, employee_family_name, employee_address_street, employee_address_postal, employee_address_city, employee_address_province, employee_address_country, user_id) values (654321, 'John', 'Doe', '321 Fake Street', 'C3B 2A1', 'Faketon', 'Fakebec', 'Fakeda', 'empmaltest');

--insert into raise (raise_date, role_type, raise_fixed_or_percent, raise_amount) values (20170801, 1, 0, 1.25);
--insert into raise (raise_date, role_type, raise_fixed_or_percent, raise_amount) values (20170801, 2, 1, 500);
--insert into raise (raise_date, role_type, raise_fixed_or_percent, raise_amount) values (20040801, 1, 0, 1.25);

--insert into role (role_id, role_start_date, role_end_date, role_salary, role_type, employee_id) values (1, 20040801, 20170701, 250, 1, 123456);
--insert into role (role_id, role_start_date, role_end_date, role_salary, role_type, employee_id) values (2, 20040801, 20170801, 250, 2, 123456);
--insert into role (role_id, role_start_date, role_salary, role_type, employee_id) values (3, 20170802, 250, 1, 123456);
--insert into role (role_id, role_start_date, role_salary, role_type, employee_id) values (4, 20040101, 10000, 1, 654321);

--insert into deduction (deduction_id, deduction_name, deduction_fixed_or_percent, deduction_amount) values (1, 'Fake Tax', 0, 11000);
--insert into deduction (deduction_id, deduction_name, deduction_fixed_or_percent, deduction_amount) values (2, 'Real Tax', 1, 11);

--insert into has_deduction (role_type, deduction_id) values (1, 1);
--insert into has_deduction (role_type, deduction_id) values (1, 2);

--insert into leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) values (1, 20170101, 20170202, 50, 1);
--insert into leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) values (2, 20170101, 20170202, 50, 4);
--insert into leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) values (3, 20160101, 20160202, 50, 4);
--insert into leave (leave_id, leave_start_date, leave_end_date, leave_percentage, role_id) values (4, 20150101, 20150202, 50, 4);

COMMIT TRANSACTION;
