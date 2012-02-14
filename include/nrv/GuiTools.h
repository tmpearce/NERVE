//GuiTools contains classes and/or functions to make using
//Qt guis easier
#pragma once
#include <QtGui\QWidget>
#include <QtGui\QTabWidget>
#include <QtCore\QStringList>
#include <QtCore\QMetaProperty>
#include <string>
#include <QtCore\QFile>
#include <QtCore\QTextStream>
#include <QtCore\QMap>
#include <QtGui\QLineEdit>
#include <QtGui\QComboBox>
#include <QtGui\QListWidget>

namespace GuiTools
{

class SerialQObject
{
public:
	SerialQObject()
	{
		serializerVersion_ = "SerializerVersion1";
		endOfTextValue_ = " _ENDOFTEXTVALUE";
		startOfTextValue_ = "STARTOFTEXTVALUE_ ";
	}
	static SerialQObject fromQObject(QObject * obj)
	{
		SerialQObject sobj;
		sobj.className_ = obj->metaObject()->className();
		sobj.objectName_ = obj->objectName();

		
		for(int i=0; i<obj->metaObject()->propertyCount(); ++i)
		{
			QMetaProperty qmp = obj->metaObject()->property(i);
			if(qmp.isUser())
			{
				sobj.propertyNames.push_back(qmp.name());
				sobj.propertyValues.push_back(qmp.read(obj).toString());

				//deal with special cases:
				//text:
				if(sobj.propertyNames.last() == "text")
				{
					sobj.propertyValues.last().prepend(sobj.startOfTextValue());
					sobj.propertyValues.last().append(sobj.endOfTextValue());
				}
			}
			
			
		}

		//special cases:
		//QComboBox:
		if(sobj.className_ == "QComboBox")
		{
			sobj.propertyNames.push_back("currentText");
			QString str = obj->property("currentText").toString();
			str.prepend(sobj.startOfTextValue());
			str.append(sobj.endOfTextValue());
			sobj.propertyValues.push_back(str);

			//also save the itemData for each item.
			//use the itemText as the propertyName and
			//the itemData as the propertyValue.
			QComboBox* cb = dynamic_cast<QComboBox*>(obj);
			if(cb)
			{
				for(int i=0; i<cb->count(); ++i)
				{
					
					QString itemData = cb->itemData(i).toString();
					if(!itemData.isEmpty())
					{
						QString itemText = cb->itemText(i);
						itemText.prepend(sobj.startOfTextValue());
						itemText.append(sobj.endOfTextValue());
						itemData.prepend(sobj.startOfTextValue());
						itemData.append(sobj.endOfTextValue());
						sobj.propertyNames.push_back(itemText);
						sobj.propertyValues.push_back(itemData);
					}					
				}
			}
			else
			{
				printf("Error: object with className==QComboBox failed to be dynamic_cast to a QComboBox\n");
			}
		}
		//QAction:
		if(sobj.className_ == "QAction")
		{
			if(obj->property("checkable").toBool())
			{
				sobj.propertyNames.push_back("checked");
				sobj.propertyValues.push_back(obj->property("checked").toString());
			}
		}
		//QListWidget
		if(sobj.className_ == "QListWidget")
		{
			QListWidget* lw = dynamic_cast<QListWidget*>(obj);
			if(lw==NULL)
			{
				printf("Error: object with className==QListWidget failed to be dynamic_cast to a QListWidget\n");
			}
			
			//iterate through items in the list, saving text and user data
			for(int i=0; i < lw->count(); ++i)
			{
				QListWidgetItem* lwi = lw->item(i);
				QString itemText = lwi->text();
				QString itemData = lwi->data(Qt::UserRole).toString();
				QString textPropName = "itemText";
				textPropName.append(QVariant(i).toString());
				QString dataPropName = "itemData";
				dataPropName.append(QVariant(i).toString());

				itemText.prepend(sobj.startOfTextValue());
				itemText.append(sobj.endOfTextValue());
				sobj.propertyNames.push_back(textPropName);
				sobj.propertyValues.push_back(itemText);

				itemData.prepend(sobj.startOfTextValue());
				itemData.append(sobj.endOfTextValue());
				sobj.propertyNames.push_back(dataPropName);
				sobj.propertyValues.push_back(itemData);
			}
		}

		return sobj;
	}
	static SerialQObject fromQString(QString str)
	{
		SerialQObject sobj;
		if(str.isEmpty()) return sobj;
		//split the line into individual words (space delimited)
		QStringList list = str.split(" ");
		if(list.size() == 0) return sobj;
		//check version of SerialQObject which wrote the line.
		if(list.last() != sobj.version())
		{
			printf("Warning: config file written by %s; This is %s\n", list.last().toAscii().constData(), sobj.version().toAscii().constData());
			return  sobj;
		}
		//version check is complete - discard version string
		list.pop_back();

		//text values may contain whitespace, and therefore are broken by
		//using split(" ").  To fix this, special keywords delimit text values.
		//combine everything between startOfTextValue and endOfTextValue words
		//into a single string, which may contain spaces
		for(int i=0; i<list.size(); ++i)
		{
			if(list.at(i) == QString(sobj.startOfTextValue()).remove(" "))
			{
				list.removeAt(i);
				//removeAt shifts everything after the current index forward
				//so i now accesses the next element (without having to increment)
				//this means element i is the start of the string between the
				//start and stop strings.  Append subsequent strings onto this one
				//and remove them until the stop string is found.
				int num_endOfTextsToSkip = 0;
				++i;
				while(i<list.size())
				{
					if(list.at(i) == QString(sobj.startOfTextValue()).remove(" "))
					{
						num_endOfTextsToSkip++;//increment this each time another startOfTextValue string is found before the matching end string is found
					}
					if(list.at(i) == QString(sobj.endOfTextValue()).remove(" "))
					{
						if(num_endOfTextsToSkip >0) 
						{
							num_endOfTextsToSkip--;//decrement each time end string is found
						}
						else//don't need to skip it, the end of the string has now been found
						{
							list.removeAt(i);
							break;
						}
					}
					list[i-1].append(" ");//add the whitespace back in.
					list[i-1].append(list.at(i));//operator[]() returns non-const reference (at(i) only returns const)
					//printf("New string: (%s)\n",list.at(i-1).toAscii().constData());
					list.removeAt(i);//removing an element changes the size of the list, so don't increment.

				}
				--i;//move back a spot in the list to continue checking for text
			}
		}
		

		if(list.size() < 2)
		{
			printf("Warning: config file line had fewer than 2 entries (excluding version number) - skipping\n");
			return sobj;
		}
		//printf("Creating SerialQObject: (%s) (%s)", list.at(0).toAscii().constData(), list.at(1).toAscii().constData());
		sobj.className_ = list.at(0);
		sobj.objectName_ = list.at(1);
		//read property-value pairs into the appropriate lists
		for(int i=2; i<list.size();)
		{
			//printf(" (%s) (%s)", list.at(i).toAscii().constData(), list.at(i+1).toAscii().constData());
			sobj.propertyNames.push_back(list.at(i));
			++i;
			sobj.propertyValues.push_back(list.at(i));
			++i;
		}
		//printf(".\n");
		return sobj;
	}
	void writeToFile(FILE* fptr)
	{
		//writeToFile requires a FILE* pointer which has been opened with write access. 

		//if either the class name or object name is blank, don't write anything -
		//there is no way to resurrect the object without these identifiers
		if(className_ == "" || objectName_ == "")
		{
			return;
		}
		fprintf_s(fptr, "%s %s ",
			className_.toAscii().constData(),
			objectName_.toAscii().constData());

		for(int i=0; i<propertyNames.size(); ++i)
		{
			fprintf_s(fptr, "%s %s ",
				propertyName(i).toAscii().constData(),
				propertyValue(i).toAscii().constData());
		}

		fprintf_s(fptr, "%s\n", version().toAscii().constData());
	}
	void appendToQString(QString* string)
	{

		//if either the class name or object name is blank, don't write anything -
		//there is no way to resurrect the object without these identifiers
		if(className_ == "" || objectName_ == "")
		{
			return;
		}
		{
			char c[500];
			sprintf_s(c, 500, "%s %s ",
				className_.toAscii().constData(),
				objectName_.toAscii().constData());
			string->append(c);
		}

		for(int i=0; i<propertyNames.size(); ++i)
		{
			char c[500];
			sprintf_s(c, 500, "%s %s ",
				propertyName(i).toAscii().constData(),
				propertyValue(i).toAscii().constData());
			string->append(c);
		}
		string->append(version());
	}

	bool setQObjectProperties(QObject * obj) 
	{
		//Make sure the serial object matches the QObject
		if(objectName_ == "" || obj == NULL)
		{
			//no object name was saved for this serial object - no way to
			//ensure it is the same widget that it was generated from.
			return false;
		}
		if(className_ != obj->metaObject()->className())
		{
			printf("Error: Class names do not match (%s and %s).  Aborting setQObjectProperties.\n",
				className_.toAscii().constData(), obj->metaObject()->className());
			return false;
		}
		if(objectName_ != obj->objectName())
		{
			printf("Error: Object names do not match (%s and %s).  Aborting setQObjectProperties.\n",
				objectName_.toAscii().constData(), obj->objectName().toAscii().constData());
			return false;
		}
		
		if(className_ == "QListWidget")
		{
			QListWidget* lw = dynamic_cast<QListWidget*>(obj);
			if(!lw)
			{
				printf("Error: object with className_ = QListWidget could not be cast to a QListWidget.\n");
				return false;
			}
			//use the acceptDrops property of the QListWidget to determine
			//how to set up the widget:  If drop is NOT enabled, the user
			//can't set up a customized list so we don't want to mess with the
			//already-initialized list.  Therefore, we look for an
			//item whose text matches the text from the config file, and add
			//the data to that item.  If drop IS enabled, the list is user-
			//configurable.  Therefore we want to clear anything that is
			//in the list and re-assemble the entire list based on the config
			//file.
			if(lw->acceptDrops())
			{
				//if dropEnabled, clear the list so it can be
				//recreated from the file
				lw->clear();
			}
		}
		//If the objects match class and object names, set the properties
		for(int i=0; i<propertyNames.size(); ++i)
		{
			//special cases first:
			//QLineEdit:
			if(className_ == "QLineEdit" && propertyName(i).compare(QString("text")))
			{
				QLineEdit* le = qobject_cast<QLineEdit*>(obj);
				if(le)
				{
					le->setText(propertyValue(i));
					QMetaObject::invokeMethod(obj, "textEdited", Q_ARG(QString, le->text()));
				}
			}
			//QComboBox:
			if(className_ == "QComboBox")
			{
				QComboBox* cb = qobject_cast<QComboBox*>(obj);
				if(!cb)
				{
					printf("Error: object with className_ = QComboBox could not be cast to a QComboBox.\n");
					return false;
				}
				//propertyName(i) is an itemText for an item in the QComboBox
				if(cb->findText(propertyName(i)) != -1)
				{
					int index = cb->findText(propertyName(i));
					cb->setItemData(index, QVariant(propertyValue(i)));
					continue;//done dealing with this property-value pair
				}
				//propertyName(i) is currentText
				if(propertyName(i) == "currentText")
				{
					cb->setCurrentIndex(cb->findText(propertyValue(i)));
					continue;//done dealing with this property-value pair
				}
				//propertyName(i) isn't an item isn't one of our special cases.
				//However, it could have been in the box when the file
				//was saved, so we need to check whether it is actually a
				//valid property.  If it isn't, continue (ignoring this non-property);
				//if it is, pass control to the code that handles non-special-cases
				if(cb->metaObject()->indexOfProperty(propertyName(i).toAscii().constData()) == -1)
				{
					continue;//done dealing with this property-value pair
				}
			}
			//QListWidget
			if(className_ == "QListWidget")
			{
				QListWidget* lw = qobject_cast<QListWidget*>(obj);
				if(!lw)
				{
					printf("Error: object with className_ = QListWidget could not be cast to a QListWidget.\n");
					return false;
				}
				//use the acceptDrops property of the QListWidget to determine
				//how to set up the widget:  If drop is NOT enabled, the user
				//can't set up a customized list so we don't want to mess with the
				//already-initialized list.  Therefore, we look for an
				//item whose text matches the text from the config file, and add
				//the data to that item.  If drop IS enabled, the list is user-
				//configurable.  Therefore we want to clear anything that is
				//in the list and re-assemble the entire list based on the config
				//file.
				
				
				if(propertyName(i).contains("currentRow") && i!=propertyNames.size()-1)
				{
					printf("Moving property/value pair %i %i\n",i,propertyNames.size());
					propertyNames.push_back(propertyName(i));
					propertyValues.push_back(propertyValue(i));
				}
				if(propertyName(i).contains("itemText"))
				{
					//QListWidgets have text-data pairs saved sequentially in the file.
					//We want to read the item's text and data together, and then
					//recreate the list widget based on these pairs.
					QString str = propertyName(i);
					str.remove("itemText");
					bool conversionSucceeded;
					int itemTextIndex = QVariant(str).toInt(&conversionSucceeded);
					if(conversionSucceeded)
					{
						
						//make sure the next property is itemDataX and that X is the same int
						//as the itemTextX that we just found.
						str = propertyName(i+1);
						str.remove("itemData");
						int itemDataIndex = QVariant(str).toInt(&conversionSucceeded);
						if(conversionSucceeded)
						{
							if(itemDataIndex == itemTextIndex)//found a matched text-data pair
							{
								QString itemText = propertyValue(i);
								QString itemData = propertyValue(i+1);
								//use the dropEnabled flag to take correct action
								if(lw->acceptDrops())
								{									
									QListWidgetItem* lwi = new QListWidgetItem();
									lwi->setText(itemText);
									lwi->setData(Qt::UserRole, itemData);
									lw->insertItem(itemTextIndex, lwi);
								}
								else
								{
									lw->findItems(itemText, Qt::MatchExactly).at(0)->setData(Qt::UserRole, itemData);
								}
							}
							else
							{
								printf("Warning: itemText and itemData indices not matched.  Skipping initialization.\n");
							}
						}
						else
						{
							printf("Warning: itemData property found but conversion to integer of item index failed.\n");
						}
					}
					else
					{
						printf("Warning: itemText property found, but conversion to integer of item index failed.\n");
					}
				}
			}

			//printf("setting object (%s) index %i of %i ", objectName_.toAscii().constData(), i, propertyNames.size());
			//printf("property (%s) to value (%s)\n",
			//	propertyName(i).toAscii().constData(),
			//	propertyValue(i).toAscii().constData());
			obj->setProperty(propertyName(i).toAscii().constData(), propertyValue(i));
		}
		
		return true;
	}
	QString const className(){return className_;}
	QString const objectName(){return objectName_;}
	QString const propertyName(int i){return propertyNames.at(i);}
	QString const propertyValue(int i){return propertyValues.at(i);}
	QString const version(){return serializerVersion_;}
	
protected:
	QString className_;
	QString objectName_;
	QStringList propertyNames;
	QStringList propertyValues;
	QString serializerVersion_;

	QString endOfTextValue_;
	QString startOfTextValue_;
	QString const endOfTextValue(){return endOfTextValue_;}
	QString const startOfTextValue(){return startOfTextValue_;}
};

};
class GuiToolsClass
{
public:
	typedef QMap<QString, QObject*> QObjectMap;

	enum SERIALIZATION_DIRECTION
	{
		READ,
		WRITE
	};
	enum SHOULD_IGNORE_QT_UNDERSCORED_NAMES
	{
		IGNORE_QT_UNDERSCORED_NAMES,
		DO_NOT_IGNORE_QT_UNDERSCORED_NAMES
	};
	
	static QObjectList getQObjectChildren(QObject * obj)
	{
		QObjectList list;
		if ( dynamic_cast<QTabWidget*>(obj) )
		{
			QTabWidget * tabWidget = (QTabWidget*)obj;
			for (int i=0; i<tabWidget->count(); i++)
			{
				list.push_back(tabWidget->widget(i));
			}
		}
		else
			list = obj->children();
		return list;
	}
	
	static QObjectList getGuiQObjectList(QObject * obj)
	{
		QObjectList list;
		list.push_back(obj);
		QObjectList children = getQObjectChildren(obj);
		for(int i=0; i<children.size(); ++i)
		{
			list.append(getGuiQObjectList(children.at(i)));
		}
		return list;
	}
	static QObjectMap getGuiQObjectMap(const QObjectList& list)
	{
		QObjectMap obj_map;
		for(int i=0; i<list.size(); ++i)
		{
			obj_map.insert(list.at(i)->objectName(), list.at(i));
		}
		return obj_map;
	}
	static void writeConfigFile(QObject * gui, std::string filename)
	{
		FILE* file;
		fopen_s(&file, filename.c_str(), "w");
		QObjectList list = getGuiQObjectList(gui);
		for(int i=0; i<list.size(); ++i)
		{
			GuiTools::SerialQObject::fromQObject(list.at(i)).writeToFile(file);			
		}
		fclose(file);
	}

	static void readConfigFile(QObject * gui, std::string filename, bool blockSignalsWhileLoading = false)
	{
		QObjectMap qmap = getGuiQObjectMap(getGuiQObjectList(gui));
		QFile f(filename.c_str());
		bool success = f.open(QIODevice::ReadOnly);
		if(!success)
		{
			printf("Failed to open file %s\n", filename.c_str());
			return;
		}
		QTextStream textstream(&f);
		while(!textstream.atEnd())
		{
			QString str = textstream.readLine();
			GuiTools::SerialQObject sobj = GuiTools::SerialQObject::fromQString(str);
			if(qmap.contains(sobj.objectName()))
			{
				QObject* qobj = qmap[sobj.objectName()];
				bool oldSignalBlockedSetting = qobj->signalsBlocked();
				qobj->blockSignals(blockSignalsWhileLoading || oldSignalBlockedSetting);
				sobj.setQObjectProperties(qobj);
				qobj->blockSignals(oldSignalBlockedSetting);
			}
		}
		f.close();
	}
	static QString writeConfigQString(QObject * obj)
	{
		QString string;
		QObjectList list = getGuiQObjectList(obj);
		for(int i=0; i<list.size(); ++i)
		{
			GuiTools::SerialQObject::fromQObject(list.at(i)).appendToQString(&string);
			string.append("_END_OF_OBJECT_");
		}
		return string;
	}

	static void readConfigQString(QObject * obj, QString * string, bool blockSignalsWhileLoading = false)
	{
		QObjectMap qmap = getGuiQObjectMap(getGuiQObjectList(obj));
		
		string->replace("_END_OF_OBJECT_", "\n");
		QTextStream textstream(string);
		while(!textstream.atEnd())
		{
			QString str = textstream.readLine();
			GuiTools::SerialQObject sobj = GuiTools::SerialQObject::fromQString(str);
			if(qmap.contains(sobj.objectName()))
			{
				QObject* qobj = qmap[sobj.objectName()];
				bool oldSignalBlockedSetting = qobj->signalsBlocked();
				qobj->blockSignals(blockSignalsWhileLoading || oldSignalBlockedSetting);
				sobj.setQObjectProperties(qobj);
				qobj->blockSignals(oldSignalBlockedSetting);
			}
		}
	}

};
