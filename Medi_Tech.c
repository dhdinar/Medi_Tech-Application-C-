#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>

void SPI(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    system("cls");
    int patient_id;
    printf("\n\t\t***Search Patients Information***\n\n");
    printf("\tEnter Patients Id: ");
    scanf("%d",&patient_id);
    system("cls");
    printf("\n\t\t***Search Patients Information***\n\n");
    char query[200];
    sprintf(query, "SELECT * FROM patients WHERE id = %d", patient_id);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "mysql_query() failed\n");
        exit(1);
    }

    rs = mysql_use_result(conn);

    if ((record = mysql_fetch_row(rs)) != NULL)
    {
        printf("\tPatient ID: %s\n", record[0]);
        printf("\tName: %s\n", record[1]);
        printf("\tAppointed Doctors Name: %s\n", record[2]);
        printf("\tAge: %s\n", record[3]);
        printf("\tGender: %s\n", record[4]);
        printf("\tContact: %s\n", record[5]);
        printf("\tBlood Group: %s\n", record[6]);
        printf("\tDisease: %s\n", record[7]);
        printf("\tPresent Condition: %s\n", record[8]);
        printf("\tRoom No: %s\n", record[9]);
        printf("\tBed No: %s\n", record[10]);
        printf("\tAdmit Date: %s\n", record[11]);
        printf("\tOT Date: %s\n", record[12]);
        printf("\tDischarged Date: %s\n", record[13]);
    }
    else
    {
        printf("No patient found with ID: %d\n", patient_id);
    }

    mysql_free_result(rs);
    printf("\n\n\tPress any key to continue......\n");
    getch();
}



int user_exists(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record,char* userid)
{


    if (mysql_query(conn, "SELECT * FROM admin"))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));

        return 1;
    }

    rs = mysql_store_result(conn);
    if (rs == NULL)
    {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(conn));

        return 1;
    }

    while ((record = mysql_fetch_row(rs)) != NULL)
    {
        if (record[1] == NULL)
        {
            printf("Something Wrong!!!\n");
            printf("Kindly check Database file.\n");
            exit(1);
        }
        else if(strcmp(record[1], userid) == 0)
        {
            printf("User Already Exists!!\n");
            mysql_free_result(rs);

            return 1;
        }
    }


    mysql_free_result(rs);

    return 0;
}


int patient_exists(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record,int id)
{
    char id_str[20];
    sprintf(id_str, "%d", id);
    if (mysql_query(conn, "SELECT * FROM patients"))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));

        return 1;
    }

    rs = mysql_store_result(conn);
    if (rs == NULL)
    {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(conn));

        return 1;
    }

    while ((record = mysql_fetch_row(rs)) != NULL)
    {

        if(strcmp(record[0],id_str) == 0)
        {
            mysql_free_result(rs);
            return 1;
        }

    }


    mysql_free_result(rs);


    return 0;
}


int login(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    system("cls");
    char userid[20];
    char pass[20];

    printf("\n\t\t***Login***\n\n");
    printf("Login id: ");
    scanf("%s",userid);
    printf("Password: ");
    scanf("%s",pass);


    // Check if the user exists
    if (!user_exists(conn,rs,record,userid))
    {
        printf("User does not exist!\n");
        return 0; // Indicate login failure
    }


    if (mysql_query(conn, "SELECT * FROM admin"))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }
    rs = mysql_store_result(conn);
    if (rs == NULL)
    {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(conn));
        mysql_close(conn);
        return 1;
    }

    while ((record = mysql_fetch_row(rs)) != NULL)
    {
        if (record[1] == NULL||record[2] == NULL)
        {
            printf("Something Wrong!!!\n");
            printf("Kindly check Database file.\n");
            exit(1);
        }
        else if(strcmp(record[1], userid)==0&&strcmp(record[2], pass)==0)
        {
            system("cls");
            printf("\t\tWelcome %s",record[0]);
            mysql_free_result(rs);

            admin_login(conn,rs,record);
            return 0;
        }
    }
    invalid();
}



void invalid()
{
    printf("Invalid Input\n");
    printf("Press any key to continue....\n");
    getch();

}



void admin_login(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{

    int p;
    printf("\n\t\t****Admin Dashboard****\n\n\n");
    printf("\t01. Add Patient\n");
    printf("\t02. Update Patient Information\n");
    printf("\t03. Modify Admin\n");
    printf("\t04. Logout\n");
    printf("\n\n\tInput your interest: ");
    scanf("%d",&p);
    switch (p)
    {
    case 1:
        add_patient(conn,rs,record);
        break;
    case 2:
        update_patient_info(conn,rs,record);
        break;

    case 3:
        modify_admin(conn,rs,record);
        break;
    case 4:
        main();
        break;
    default:
        invalid();
        admin_login(conn,rs,record);
    }
}



void add_patient(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    system("cls");
    int *id,*age,x,n;
    char *name[30],*a_doctor[30],*gender[7],*mobile[15],*b_group[5],*disease[30],*condition[50],*room [10],*bed [8],*admit_date[10],*ot_date[10],*discharge_date[10];


    printf("\n\t\t   ***Add New Patient***\n\n\n");
    printf("\tEnter Patients Id: ");
    scanf("%d",&id);
    fflush(stdin);
    x=patient_exists(conn,rs,record,id);
    if(x==1)
    {
        printf("Patient already Exist in this ID!!\n");
        invalid();
        add_patient(conn,rs,record);
    }

    printf("\tEnter Patients Name: ");
    gets(name);
    printf("\tEnter Appointed Doctors Name: ");
    gets(a_doctor);
    printf("\tEnter Patients Age: ");
    scanf("%d",&age);
    fflush(stdin);
    printf("\tEnter Gender: ");
    gets(gender);
    printf("\tEnter Mobile No: ");
    gets(mobile);
    printf("\tEnter Blood Group: ");
    gets(b_group);
    printf("\tPatients Disease: ");
    gets(disease);
    printf("\tPatients Condition: ");
    gets(condition);
    printf("\tPatients Room No: ");
    gets(room);
    printf("\tPatients Bed No: ");
    gets(bed);
    printf("\tEnter Admit Date(YYYY-MM-DD): ");
    gets(admit_date);
    printf("\tEnter Operation Date(YYYY-MM-DD): ");
    gets(ot_date);
    printf("\tEnter Discharge Date(YYYY-MM-DD): ");
    gets(discharge_date);


    char query[200]; // Adjust the size according to your needs
    snprintf(query, sizeof(query), "INSERT INTO patients VALUES(%d, '%s','%s', %d, '%s', '%s', '%s','%s','%s', '%s', '%s', '%s', '%s', '%s')",
             id, name,a_doctor, age, gender, mobile, b_group, disease, condition,room,bed, admit_date, ot_date, discharge_date);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));

        return ;
    }

    printf("\tData Submitted Successfully!!\n\n");
    printf("\t01. Add New Patient.\n");
    printf("\t02. Admin Dashboard.");
    printf("\n\n\tInput your interest: ");
    scanf("%d",&n);
    switch(n)
    {
    case 1:
        add_patient(conn,rs,record);
        break;
    case 2:
        system("cls");
        admin_login(conn,rs,record);
        break;
    default:
        invalid();
        main();
    }

}



void update_patient_info(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    system("cls");
    int *id,*age,x,n;
    char *name[30],*a_doctor[30],*gender[7],*mobile[15],*b_group[5],*disease[30],*condition[50],*room [10],*bed [8],*admit_date[10],*ot_date[10],*discharge_date[10];


    printf("\n\t\t   ***Update Patient Informations***\n\n\n");
    printf("\tEnter Patients Id: ");
    scanf("%d",&id);
    fflush(stdin);
    x=patient_exists(conn,rs,record,id);
    if(x==0)
    {
        printf("No patient Available in this ID!!!\n");
        invalid();
        update_patient_info(conn,rs,record);
    }
    printf("\tEnter Patients Name: ");
    gets(name);
    printf("\tEnter Appointed Doctors Name: ");
    gets(a_doctor);
    printf("\tEnter Patients Age: ");
    scanf("%d",&age);
    fflush(stdin);
    printf("\tEnter Gender: ");
    gets(gender);
    printf("\tEnter Mobile No: ");
    gets(mobile);
    printf("\tEnter Blood Group: ");
    gets(b_group);
    printf("\tPatients Disease: ");
    gets(disease);
    printf("\tPatients Condition: ");
    gets(condition);
    printf("\tPatients Room No: ");
    gets(room);
    printf("\tPatients Bed No: ");
    gets(bed);
    printf("\tEnter Admit Date(YYYY-MM-DD): ");
    gets(admit_date);
    printf("\tEnter Operation Date(YYYY-MM-DD): ");
    gets(ot_date);
    printf("\tEnter Discharge Date(YYYY-MM-DD): ");
    gets(discharge_date);


    char query[1000];
    snprintf(query, sizeof(query), "UPDATE patients SET id=%d, name='%s',a_doctor='%s' age=%d, gender='%s', contact='%s', blood_group='%s', disease='%s', p_condition='%s',room='%s',bed='%s', admit_date='%s', ot_date='%s', discharged_date='%s'WHERE id=%d",
             id, name,a_doctor, age, gender, mobile, b_group, disease, condition,room,bed, admit_date, ot_date, discharge_date,id);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));
    }
    else
    {
        printf("\tData Updated Successfully!!\n\n");
    }
    printf("\t01. Upadate more Patient.\n");
    printf("\t02. Admin Dashboard.");
    printf("\n\n\tInput your interest: ");
    scanf("%d",&n);
    switch(n)
    {
    case 1:
        update_patient_info(conn,rs,record);
        break;
    case 2:
        system("cls");
        admin_login(conn,rs,record);
        break;
    default:
        invalid();
        main();
    }


}



void delete_admin(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    char *userid[12];
    fflush(stdin);
    printf("\tUser Name Of Admin you want to delete: ");
    gets(userid);
    if (!user_exists(conn,rs,record,userid))
    {
        printf("User does not exist!\n");
        //return ; // Indicate login failure
    }
    char query[100];
    snprintf(query, sizeof(query), "DELETE FROM admin WHERE userid='%s'", userid);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Error in deleting admin: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Admin deleted successfully!\n");
        printf("Press any key to continue.....");
        getch();
        modify_admin(conn,rs,record);
    }
}


void add_admin(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    char *name[30],*userid[10],*password[10];
    printf("\n\n\t\t\t     ***Add New Admin***\n\n");
    fflush(stdin);
    printf("\tEnter a Name: ");
    gets(name);
    printf("\tEnter a User Name (Maximum 9 Character): ");
    gets(userid);

    printf("\tEnter a password (Maximum 8 Character): ");
    gets(password);
    char query[100];
    snprintf(query, sizeof(query), "INSERT INTO admin (name, userid, pass) VALUES ('%s', '%s', '%s')", name, userid, password);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Error in adding admin: %s\n", mysql_error(conn));
    }
    else
    {
        printf("Admin added successfully!\n");
        printf("Press any key to continue.....");
        getch();
        modify_admin(conn,rs,record);
    }
}


int modify_admin(MYSQL *conn,MYSQL_RES *rs,MYSQL_ROW record)
{
    system("cls");
    int n;
    printf("\n\n\t\t\t     ***Admin List***\n\n");
    printf("\tAdmin Name\t\t\tUser Name\t Password\n");
    printf("\t------------------------------------------------------------\n");


    if (mysql_query(conn, "SELECT * FROM admin"))
    {
        fprintf(stderr, "Error in query: %s\n", mysql_error(conn));

        return 1;
    }
    rs = mysql_store_result(conn);
    if (rs == NULL)
    {
        fprintf(stderr, "Error storing result: %s\n", mysql_error(conn));

        return 1;
    }

    while ((record = mysql_fetch_row(rs)) != NULL)
    {
        printf("\t%-31s %-16s %s\n",record[0],record[1],record[2]);
    }

    mysql_free_result(rs);


    printf("\t------------------------------------------------------------\n\n");
    printf("\n\t01. Create New Admin.\n");
    printf("\t02. Delete Admin.\n");
    printf("\t03. Admin Dashboard.\n");
    printf("\n\tInput your interest: ");
    scanf("%d",&n);
    switch(n)
    {
    case 1:
        add_admin(conn,rs,record);
        break;
    case 2:
        delete_admin(conn,rs,record);
        break;
    case 3:
        system("cls");
        admin_login(conn,rs,record);
        break;
    default:
        invalid();
        modify_admin(conn,rs,record);
    }
}




int main()
{

    MYSQL *conn;
    MYSQL_RES *rs;
    MYSQL_ROW record;
    char *server=" ";//Give your server address
    char *user=" ";// give user name
    char *password=" ";// password
    char *database=" ";// database name
    conn=mysql_init(NULL);
    conn=mysql_init(0);
    conn=mysql_real_connect(conn,server,user,password,database,3306,NULL,0);

    if(conn)
        printf("Database Connection Established!!\n");

    else
    {
        printf("Error Establishing Database connection!!\n");
        return 1;
    }



    int choice;
    do
    {
        system("cls");
        printf("\n\t\t\***Welcome to Medi_Tech Application***\n\n\n");
        printf("\t01. Search Patients Information\n");
        printf("\t02. Admin Login\n");
        printf("\t03. Exit.");
        printf("\n\n\tInput your interest: ");
        scanf("%d",&choice);


        switch(choice)
        {

        case 1:
            SPI(conn,rs,record);
            break;

        case 2:
            login(conn,rs,record);
            break;

        case 3:
            printf("\nYou Have Exited!!");
            break;

        default :
            printf("\n\tInvalid Choice! Try Again");
            printf("\n\t press any key to continue....");
            getch();

        }
    }
    while(choice!=3);


    mysql_close(conn);
}
