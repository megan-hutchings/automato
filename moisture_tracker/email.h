#ifndef EMAIL_H
#define EMAIL_H

#include <ESP_Mail_Client.h>

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "plant.updates1@gmail.com"
#define AUTHOR_PASSWORD "insert email password here"

/* Recipient's email*/
#define RECIPIENT_EMAIL "mhg@akkodis.se"

/* Declare the global used SMTPSession object for SMTP transport */
extern SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);

void sendEmail(String plantName);

#endif