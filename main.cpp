#include <QApplication>
#include <QMimeData>
#include <QClipboard>
#include <qvariant.h>
#include <QImage>
#include <QFile>
#include <time.h>
#include <QStringList>

void writeTextFile(QString folder, QString prefix, QString ext, const QString& text) {
	QFile file(folder + "/" + prefix + QString::number(qrand() % 999999) + "." + ext);
	qDebug("Writing text clipboard to file %s", file.fileName().toLatin1().constData());

	if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QByteArray rawText = text.toUtf8();
		file.write(rawText.constData(), rawText.size());
		file.close();
	} else {
		qWarning("Failed to write file");
	}
}

void writeImageFile(QString folder, QString prefix, QString ext, const QImage& image) {
	QFile file(folder + "/" + prefix + QString::number(qrand() % 999999) + "." + ext);
	qDebug("Writing image clipboard to file %s", file.fileName().toLatin1().constData());

	if(file.open(QIODevice::WriteOnly)) {
		image.save(&file);
		file.close();
	} else {
		qWarning("Failed to write file");
	}
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	if(a.arguments().size() <= 1) {
		qFatal("Need at least one argument containing the target path to paste data");
		return 0;
	}

	qsrand(time(nullptr));

	QString folder = a.arguments()[1];
	const QClipboard *clipboard = QApplication::clipboard();
	const QMimeData *mimeData = clipboard->mimeData();

	if (mimeData->hasImage()) {
		QImage image = qvariant_cast<QImage>(mimeData->imageData());
		writeImageFile(folder, "img", "png", image);
	} else if (mimeData->hasText()) {
		QString text = mimeData->text();
		writeTextFile(folder, "txt", "txt", text);
	} else {
		qDebug("No available clipboard data as text or image");
	}

	return 0;
}
