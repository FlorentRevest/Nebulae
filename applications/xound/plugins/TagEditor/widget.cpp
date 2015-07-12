#include "widget.h"

TagEditorPlugin::TagEditorPlugin()
{
    m_name = tr("Tags Editor").toAscii();

    QTranslator *translation = new QTranslator();
    translation->load(QString("TagEditor_") + QLocale::system().name().section('_', 0, 0));
    qApp->installTranslator(translation);

    QGridLayout *layout = new QGridLayout();
    layout->setContentsMargins(2, 2, 2, 2);

    label = new QLabel(this);
    label->setText(tr("Title:"));
    layout->addWidget(label, 0, 1, 1, 1);
    label_2 = new QLabel(this);
    label_2->setText(tr("Album:"));
    layout->addWidget(label_2, 1, 1, 1, 1);
    label_3 = new QLabel(this);
    label_3->setText(tr("Artist:"));
    layout->addWidget(label_3, 2, 1, 1, 1);
    label_4 = new QLabel(this);
    label_4->setText(tr("Track Number:"));
    layout->addWidget(label_4, 3, 1, 1, 1);
    label_5 = new QLabel(this);
    label_5->setText(tr("Year:"));
    layout->addWidget(label_5, 4, 1, 1, 1);
    label_6 = new QLabel(this);
    label_6->setText(tr("Genre:"));
    layout->addWidget(label_6, 5, 1, 1, 1);
    label_7 = new QLabel(this);
    label_7->setText(tr("Comment:"));
    layout->addWidget(label_7, 6, 1, 1, 1);

    titleEdit = new QLineEdit(this);
    layout->addWidget(titleEdit, 0, 2, 1, 3);
    albumEdit = new QLineEdit(this);
    layout->addWidget(albumEdit, 1, 2, 1, 3);
    artistEdit = new QLineEdit(this);
    layout->addWidget(artistEdit, 2, 2, 1, 3);
    trackEdit = new QSpinBox(this);
    trackEdit->setMinimum(1);
    layout->addWidget(trackEdit, 3, 2, 1, 3);
    yearEdit = new QSpinBox(this);
    yearEdit->setMaximum(3000);
    layout->addWidget(yearEdit, 4, 2, 1, 3);
    genreEdit = new QLineEdit(this);
    layout->addWidget(genreEdit, 5, 2, 1, 3);
    commentEdit = new QLineEdit(this);
    layout->addWidget(commentEdit, 6, 2, 1, 3);

    horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    layout->addItem(horizontalSpacer, 7, 1, 1, 2);
    reinitialiseButton = new QPushButton(this);
    reinitialiseButton->setText(tr("Reinitialise"));
    connect(reinitialiseButton, SIGNAL(clicked()), this, SLOT(reinitialise()));
    layout->addWidget(reinitialiseButton, 7, 3, 1, 1);
    saveButton = new QPushButton(this);
    saveButton->setText(tr("Save"));
    connect(saveButton, SIGNAL(clicked()), this, SLOT(save()));
    layout->addWidget(saveButton, 7, 4, 1, 1);

    setLayout(layout);

    f = new TagLib::FileRef();
}

QString TagEditorPlugin::name() const
{
    return m_name;
}

QIcon TagEditorPlugin::icon() const
{
    return QIcon(":/tagEditor.png");
}

void TagEditorPlugin::setMusicPath(QString currentMusicPath)
{
    f = new TagLib::FileRef(currentMusicPath.toAscii());

    if(!f->isNull() && f->tag())
    {
        TagLib::Tag *tag = f->tag();
        m_title = QString(tag->title().toCString());
        m_artist = QString(tag->artist().toCString());
        m_album = QString(tag->album().toCString());
        m_genre = QString(tag->genre().toCString());
        m_comment = QString(tag->comment().toCString());
        m_year = tag->year();
        m_track = tag->track();
    }
    titleEdit->setText(m_title);
    artistEdit->setText(m_artist);
    albumEdit->setText(m_album);
    genreEdit->setText(m_genre);
    commentEdit->setText(m_comment);
    yearEdit->setValue(m_year);
    trackEdit->setValue(m_track);
}

void TagEditorPlugin::reinitialise()
{
    titleEdit->setText(m_title);
    artistEdit->setText(m_artist);
    albumEdit->setText(m_album);
    genreEdit->setText(m_genre);
    commentEdit->setText(m_comment);
    yearEdit->setValue(m_year);
    trackEdit->setValue(m_track);
}

void TagEditorPlugin::save()
{
    if(!f->isNull() && f->tag())
    {
        TagLib::Tag *tag = f->tag();
        tag->setTitle(titleEdit->text().toAscii().data());
        tag->setArtist(artistEdit->text().toAscii().data());
        tag->setAlbum(albumEdit->text().toAscii().data());
        tag->setGenre(genreEdit->text().toAscii().data());
        tag->setComment(commentEdit->text().toAscii().data());
        tag->setYear(yearEdit->value());
        tag->setTrack(trackEdit->value());
        f->save();
        m_title = QString(tag->title().toCString());
        m_artist = QString(tag->artist().toCString());
        m_album = QString(tag->album().toCString());
        m_genre = QString(tag->genre().toCString());
        m_comment = QString(tag->comment().toCString());
        m_year = tag->year();
        m_track = tag->track();
    }
}

Q_EXPORT_PLUGIN2(TagEditorPlugin, TagEditorPlugin)
