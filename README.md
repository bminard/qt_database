# Qt Database

Experiments with Qt's SQLite database driver.

---

Updated original example by Davide Coppela to include classes and a schema (based upon the Composite design pattern).

The motivation for this change was the recognition that database migrations could be handled using the Composite design pattern.
In this implementation, the Component is the 'Schema', the Composite the 'TableRevisionHistory' and the Leaf the 'TablleRevision'.

A table revision history is a list of table revisions executed on the database.
In effect this call can be used to provide a database migration in one direction (forward).

The use case I was exploring involved the following.
- Application A, revision 1 uses database schema D, revision 1 and is deployed.
- Later, Application A, revision 2 using database schema D, revision 2 is deployed.

When Application revision 2 is available the following situation results.
- New users install revision 2 without trouble.
- Users upgrading from revision 1 don't have the correct database schema to support revision 2.

What to do?

The Schema class provides a way of managing database migrations for users upgrading from revision 1.

---

Example by Davide Coppola. (With Thanks!)
See his Article at [How to embed a database in your application with SQLite and Qt](http://blog.davidecoppola.com/2016/11/howto-embed-database-in-application-with-sqlite-and-qt/).
Davide's source code at [GitHub](https://github.com/vivaladav/BitsOfBytes/blob/master/howto-embed-database-in-application-with-sqlite-and-qt/QtSqlExample/MainWindow.cpp).
