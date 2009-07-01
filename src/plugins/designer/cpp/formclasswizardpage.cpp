/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** Commercial Usage
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://www.qtsoftware.com/contact.
**
**************************************************************************/

#include "formclasswizardpage.h"
#include "ui_formclasswizardpage.h"
#include "formclasswizardparameters.h"

#include <coreplugin/icore.h>
#include <coreplugin/mimedatabase.h>
#include <cppeditor/cppeditorconstants.h>
#include <cpptools/cpptoolsconstants.h>

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <QtGui/QAbstractButton>
#include <QtGui/QMessageBox>

namespace Designer {
namespace Internal {

// ----------------- FormClassWizardPage

FormClassWizardPage::FormClassWizardPage(QWidget * parent) :
    QWizardPage(parent),
    m_ui(new Ui::FormClassWizardPage),
    m_isValid(false)
{
    m_ui->setupUi(this);

    m_ui->newClassWidget->setBaseClassInputVisible(false);
    m_ui->newClassWidget->setNamespacesEnabled(true);
    m_ui->newClassWidget->setAllowDirectories(true);

    connect(m_ui->newClassWidget, SIGNAL(validChanged()), this, SLOT(slotValidChanged()));
    connect(m_ui->settingsToolButton, SIGNAL(clicked()), this, SLOT(slotSettings()));
}

FormClassWizardPage::~FormClassWizardPage()
{
    delete m_ui;
}

// Retrieve settings of CppTools plugin.
static  bool inline lowerCaseFiles(const Core::ICore *core)
{
    QString lowerCaseSettingsKey = QLatin1String(CppTools::Constants::CPPTOOLS_SETTINGSGROUP);
    lowerCaseSettingsKey += QLatin1Char('/');
    lowerCaseSettingsKey += QLatin1String(CppTools::Constants::LOWERCASE_CPPFILES_KEY);

    return core->settings()->value(lowerCaseSettingsKey, QVariant(false)).toBool();
}

// Set up new class widget from settings
void FormClassWizardPage::initParameters()
{
    Core::ICore *core = Core::ICore::instance();
    const Core::MimeDatabase *mdb = core->mimeDatabase();
    m_ui->newClassWidget->setHeaderExtension(mdb->preferredSuffixByType(QLatin1String(CppTools::Constants::CPP_HEADER_MIMETYPE)));
    m_ui->newClassWidget->setSourceExtension(mdb->preferredSuffixByType(QLatin1String(CppTools::Constants::CPP_SOURCE_MIMETYPE)));
    m_ui->newClassWidget->setLowerCaseFiles(lowerCaseFiles(core));
}

void FormClassWizardPage::slotSettings()
{
    const QString id = QLatin1String(CppTools::Constants::CPP_SETTINGS_ID);
    const QString cat = QLatin1String(CppTools::Constants::CPP_SETTINGS_CATEGORY);
    if (Core::ICore::instance()->showOptionsDialog(cat, id, this)) {
        initParameters();
        m_ui->newClassWidget->triggerUpdateFileNames();
    }
}

void FormClassWizardPage::setClassName(const QString &suggestedClassName)
{
    // Is it valid, now?
    m_ui->newClassWidget->setClassName(suggestedClassName);
    slotValidChanged();
}

QString FormClassWizardPage::path() const
{
    return m_ui->newClassWidget->path();
}

void FormClassWizardPage::setPath(const QString &p)
{
    m_ui->newClassWidget->setPath(p);
}

void FormClassWizardPage::getParameters(FormClassWizardParameters *p) const
{
    p->setClassName(m_ui->newClassWidget->className());
    p->setPath(path());
    p->setSourceFile(m_ui->newClassWidget->sourceFileName());
    p->setHeaderFile(m_ui->newClassWidget->headerFileName());
    p->setUiFile(m_ui->newClassWidget-> formFileName());
}

void FormClassWizardPage::slotValidChanged()
{
    const bool validNow = m_ui->newClassWidget->isValid();
    if (m_isValid != validNow) {
        m_isValid = validNow;
        emit completeChanged();
    }
}

bool FormClassWizardPage::isComplete() const
{
    return m_isValid;
}

bool FormClassWizardPage::validatePage()
{
    QString errorMessage;
    const bool rc = m_ui->newClassWidget->isValid(&errorMessage);
    if (!rc) {
        QMessageBox::critical(this, tr("%1 - Error").arg(title()), errorMessage);
    }
    return rc;
}

} // namespace Internal
} // namespace Designer
