/*
 * This file is part of QtEmu project.
 * Copyright (C) 2017-2018 Sergio Carlavilla <carlavilla @ mailbox.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

// Local
#include "machine.h"

/**
 * @brief Machine object
 * @param parent, parent widget
 *
 * This is the heart of the application
 */
Machine::Machine(QObject *parent) : QObject(parent)
{
    this->m_machineProcess = new QProcess(this);

#ifdef Q_OS_WIN
    this->m_machineTcpSocket = new QTcpSocket(this);
    connect(m_machineTcpSocket, &QTcpSocket::readyRead,
            this, &Machine::readMachineStandardOut);
#endif
    connect(m_machineProcess, &QProcess::readyReadStandardOutput,
            this, &Machine::readMachineStandardOut);
    connect(m_machineProcess, &QProcess::readyReadStandardError,
            this, &Machine::readMachineErrorOut);
    connect(m_machineProcess, &QProcess::started,
            this, &Machine::machineStarted);
    connect(m_machineProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Machine::machineFinished);

    qDebug() << "Machine object created";
}

Machine::~Machine()
{
    qDebug() << "Machine object destroyed";
}

/**
 * @brief Get the name of the machine
 *
 * Get the name of the machine
 */
QString Machine::getName() const
{
    return name;
}

/**
 * @brief Set the name of the machine
 *
 * Set the name of the machine
 */
void Machine::setName(const QString &value)
{
    name = value;
}

/**
 * @brief Get the operating system type of the machine
 *
 * Get the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
QString Machine::getOSType() const
{
    return OSType;
}

/**
 * @brief Set the operating system type of the machine
 *
 * Set the operating system type of the machine.
 * Ex: GNU/Linux, Windows...
 */
void Machine::setOSType(const QString &value)
{
    OSType = value;
}

/**
 * @brief Get the operating system version of the machine
 *
 * Get the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
QString Machine::getOSVersion() const
{
    return OSVersion;
}

/**
 * @brief Set the operating system version of the machine
 *
 * Set the operating system version of the machine.
 * Ex: Debian, Mageia, Arch Linux...
 */
void Machine::setOSVersion(const QString &value)
{
    OSVersion = value;
}

/**
 * @brief Get the machine path
 *
 * Get the machine path
 * Ex: /home/xexio/Vms/Debian
 */
QString Machine::getPath() const
{
    return path;
}

/**
 * @brief Set the machine path
 *
 * Set the machine path
 * Ex: /home/xexio/Vms/Debian
 */
void Machine::setPath(const QString &value)
{
    path = value;
}

/**
 * @brief Get the configuration path
 *
 * Get the configuration path
 */
QString Machine::getConfigPath() const
{
    return configPath;
}

/**
 * @brief Set the configuration path
 *
 * Set the configuration path
 */
void Machine::setConfigPath(const QString &value)
{
    configPath = value;
}

/**
 * @brief Get the machine uuid
 *
 * get the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
QString Machine::getUuid() const
{
    return uuid;
}

/**
 * @brief Set the machine uuid
 *
 * Set the machine uuid with a unique key generated
 * with QUuid
 * Ex: {fc6a2dd5-3c31-401f-a9c7-86ad6190a77f}
 */
void Machine::setUuid(const QString &value)
{
    uuid = value;
}

/**
 * @brief Get the machine state
 *
 * Get the machine state
 * Ex: Started, Stopped or Rebooted
 */
Machine::States Machine::getState() const
{
    return state;
}

/**
 * @brief Set the machine state
 *
 * Set the machine state
 * Ex: Started, Stopped or Rebooted
 */
void Machine::setState(const States &value)
{
    state = value;
}

/**
 * @brief Get the machine descripcion
 *
 * Get the machine descripcion
 */
QString Machine::getDescription() const
{
    return description;
}

/**
 * @brief Set the machine descripcion
 *
 * Set the machine descripcion
 */
void Machine::setDescription(const QString &value)
{
    description = value;
}

/**
 * @brief Get the CPU Type of the machine
 *
 * Get the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
QString Machine::getCPUType() const
{
    return CPUType;
}

/**
 * @brief Set the CPU Type of the machine
 *
 * Set the CPU Type of the machine
 * Ex: core2duo, pentium, Opteron_G5...
 */
void Machine::setCPUType(const QString &value)
{
    CPUType = value;
}

/**
 * @brief Get the CPU Count of the machine
 *
 * Get the CPU Count of the machine
 */
int Machine::getCPUCount() const
{
    return CPUCount;
}

/**
 * @brief Set the CPU Count of the machine
 *
 * Set the CPU Count of the machine
 */
void Machine::setCPUCount(const int &value)
{
    CPUCount = value;
}

/**
 * @brief Get the CPU Socket Count of the machine
 *
 * Get the CPU Socket Count of the machine
 */
int Machine::getSocketCount() const
{
    return socketCount;
}

/**
 * @brief Set the CPU Socket Count of the machine
 *
 * Set the CPU Socket Count of the machine
 */
void Machine::setSocketCount(const int &value)
{
    socketCount = value;
}

/**
 * @brief Get the CPU Cores per Socket of the machine
 *
 * Get the CPU Cores per Socket of the machine
 */
int Machine::getCoresSocket() const
{
    return coresSocket;
}

/**
 * @brief Set the CPU Cores per Socket of the machine
 *
 * Set the CPU Cores per Socket of the machine
 */
void Machine::setCoresSocket(const int &value)
{
    coresSocket = value;
}

/**
 * @brief Get the CPU Threads per Core of the machine
 *
 * Get the CPU Threads per Core of the machine
 */
int Machine::getThreadsCore() const
{
    return threadsCore;
}

/**
 * @brief Set the CPU Threads per Core of the machine
 *
 * Set the CPU Threads per Core of the machine
 */
void Machine::setThreadsCore(const int &value)
{
    threadsCore = value;
}

/**
 * @brief Get the max hot CPUs of the machine
 *
 * Get the max hot CPUs of the machine
 */
int Machine::getMaxHotCPU() const
{
    return maxHotCPU;
}

/**
 * @brief Set the max hot CPUs of the machine
 *
 * Set the max hot CPUs of the machine
 */
void Machine::setMaxHotCPU(const int &value)
{
    maxHotCPU = value;
}

/**
 * @brief Get the GPU of the machine
 *
 * Get the GPU of the machine
 * Ex: std, vmware, cg3...
 */
QString Machine::getGPUType() const
{
    return GPUType;
}

/**
 * @brief Set the GPU of the machine
 *
 * Set the GPU of the machine
 * Ex: std, vmware, cg3...
 */
void Machine::setGPUType(const QString &value)
{
    GPUType = value;
}

/**
 * @brief Get the keyboard of the machine
 *
 * Get the keyboard of the machine
 * Ex: ar, es, hr...
 */
QString Machine::getKeyboard() const
{
    return keyboard;
}

/**
 * @brief Set the keyboard of the machine
 *
 * Set the keyboard of the machine
 * Ex: ar, es, hr...
 */
void Machine::setKeyboard(const QString &value)
{
    keyboard = value;
}

/**
 * @brief Get the RAM of the machine
 *
 * Get the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
qlonglong Machine::getRAM() const
{
    return RAM;
}

/**
 * @brief Set the RAM of the machine
 *
 * Set the RAM of the machine.
 * There's a limit in the RAM that the machine can use.
 * You can't use more RAM that your system have.
 *
 * Ex: If your computer have 8GiB of RAM, your machine
 * are limited to that amount of RAM
 */
void Machine::setRAM(const qlonglong &value)
{
    RAM = value;
}

/**
 * @brief Get the audio cards of the machine
 *
 * Get the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
QStringList Machine::getAudio() const
{
    return audio;
}

/**
 * @brief Set the audio cards of the machine
 *
 * Set the audio cards of the machine
 * Ex: ac97, es1370, hda...
 */
void Machine::setAudio(const QStringList &value)
{
    audio = value;
}

/**
 * @brief Get if the machine have network
 *
 * Get if the machine have network
 */
bool Machine::getUseNetwork() const
{
    return useNetwork;
}

/**
 * @brief Set if the machine have network
 *
 * Set if the machine have network
 */
void Machine::setUseNetwork(bool value)
{
    useNetwork = value;
}

/**
 * @brief Get the list of media
 * @return media list
 *
 * Get the list of media of the machine
 */
QList<Media> Machine::getMedia() const
{
    return media;
}

/**
 * @brief Add media to the media list
 * @param value, new media
 *
 * Add media to the media list
 */
void Machine::addMedia(const Media value)
{
    this->media.append(value);
}

/**
 * @brief Get the accelerator machine
 *
 * Get the accelerator machine
 * Ex: kvm, xen...
 */
QStringList Machine::getAccelerator() const
{
    return accelerator;
}

/**
 * @brief Set the accelerator machine
 *
 * Set the accelerator machine
 * Ex: kvm, xen...
 */
void Machine::setAccelerator(const QStringList &value)
{
    accelerator = value;
}

/**
 * @brief Get the machine boot
 *
 * Get the machine boot
 */
Boot Machine::getBoot() const {
    return m_boot;
}

/**
 * @brief Set the machine boot
 *
 * Set the machine boot
 */
void Machine::setBoot(const Boot &boot) {
    m_boot = boot;
}

/**
 * @brief Get the host sound system
 * @return the host sound system
 *
 * Get the host sound system
 * In GNU/Linux: Alsa, pa, etc...
 * In Windows: wav...
 */
QString Machine::getHostSoundSystem() const
{
    return hostSoundSystem;
}

/**
 * @brief Set the host sound system
 * @param value, new host sound system
 *
 * Set the host sound system
 */
void Machine::setHostSoundSystem(const QString &value)
{
    hostSoundSystem = value;
}

// Methods
/**
 * @brief Add the audio card to the list
 * @param audio, audio card to be added
 *
 * Add the audio card to the list
 */
void Machine::addAudio(const QString audio)
{
    if (!this->audio.contains(audio)) {
        this->audio.append(audio);
    }
}

/**
 * @brief Remove the audio card from the list
 * @param audio, audio card to be removed
 *
 * Remove the audio card from the list
 */
void Machine::removeAudio(const QString audio)
{
    if (this->audio.contains(audio)) {
        this->audio.removeOne(audio);
    }
}

/**
 * @brief Remove all audio cards
 *
 * Remove all audio cards of the machine
 */
void Machine::removeAllAudioCards()
{
    this->audio.clear();
}

/**
 * @brief Add an accelerator to the list
 * @param accel code of the accelerator
 *
 * Add an accelerator to the list
 */
void Machine::addAccelerator(const QString accel)
{
    if (!this->accelerator.contains(accel)) {
        this->accelerator.append(accel);
    }
}

/**
 * @brief Remove an accelerator from the list
 * @param accel code of the accelerator
 *
 * Remove an accelerator from the list if exists
 */
void Machine::removeAccelerator(const QString accel)
{
    if(this->accelerator.contains(accel)){
        this->accelerator.removeOne(accel);
    }
}

/**
 * @brief Remove all accelerators
 *
 * Remove all accelerators of the machine
 */
void Machine::removeAllAccelerators()
{
    this->accelerator.clear();
}

/**
 * @brief Remove all media
 *
 * Remove all media
 */
void Machine::removeAllMedia()
{
    this->media.clear();
}

/**
 * @brief Get all the audio cards separated by commas
 * @return Audio cards separated by commas
 *
 * Get all the audio cards separated by commas
 * Ex: ac97, es1370, hda
 */
QString Machine::getAudioLabel()
{
    QHash<QString, QString> soundCardsHash = SystemUtils::getSoundCards();
    QStringList audioCards = this->audio;
    for(int i = 0; i < audioCards.size(); ++i) {
        audioCards.replace(i, soundCardsHash.value(audioCards.at(i)));
    }
    QString audioLabel = audioCards.join(",");

    return audioLabel;
}

/**
 * @brief Get all the accelerators separated by commas
 * @return Accelerators separated by commas
 *
 * Get all the accelerators separated by commas
 * Ex: kvm,tcg
 */
QString Machine::getAcceleratorLabel()
{
    QHash<QString, QString> acceleratorsHash = SystemUtils::getAccelerators();
    QStringList accel = this->accelerator;
    for(int i = 0; i < accel.size(); ++i) {
        accel.replace(i, acceleratorsHash.value(accel.at(i)));
    }
    QString acceleratorLabel = accel.join(",");

    return acceleratorLabel;
}

/**
 * @brief Run the machine in QEMU
 *
 * Run the machine in QEMU process
 */
void Machine::runMachine(QEMU *QEMUGlobalObject)
{
    QStringList args = this->generateMachineCommand();

    QString program;
    #ifdef Q_OS_LINUX
    program.append(QEMUGlobalObject->getQEMUBinary("qemu-system-x86_64"));
    #endif
    #ifdef Q_OS_WIN
    program.append(QEMUGlobalObject->getQEMUBinary("qemu-system-x86_64.exe"));
    #endif
    #ifdef Q_OS_MACOS
    program.append(QEMUGlobalObject->getQEMUBinary("qemu-system-x86_64"));
    #endif
    #ifdef Q_OS_FREEBSD
    program.append(QEMUGlobalObject->getQEMUBinary("qemu-system-x86_64"));
    #endif

    if (program.isEmpty()) {
        m_machineBinaryErrorMessageBox = new QMessageBox();
        m_machineBinaryErrorMessageBox->setWindowTitle(tr("QEMU - Binary not found"));
        m_machineBinaryErrorMessageBox->setIcon(QMessageBox::Information);
        m_machineBinaryErrorMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_machineBinaryErrorMessageBox->setText(tr("QEMU binary not found"));
        m_machineBinaryErrorMessageBox->show();
        m_machineBinaryErrorMessageBox->raise();
        m_machineBinaryErrorMessageBox->activateWindow();
    }
    this->m_machineProcess->start(program, args);
#ifdef Q_OS_WIN
    QSettings settings;
    settings.beginGroup("Configuration");

    QString monitorHostName = settings.value("qemuMonitorHost", "localhost").toString();
    quint16 monitorSocket = static_cast<quint16>(settings.value("qemuMonitorPort", 6000).toInt());

    this->m_machineTcpSocket->connectToHost(monitorHostName, monitorSocket, QIODevice::ReadWrite);
#endif
}

/**
 * @brief Stop the machine
 *
 * Stop the machine
 */
void Machine::stopMachine()
{
#ifdef Q_OS_WIN
    if (this->m_machineTcpSocket->state() != QAbstractSocket::ConnectedState) {
        this->failConnectMachine();
    } else {
        this->m_machineTcpSocket->write(qPrintable("system_powerdown\n"));
    }
#else
    this->m_machineProcess->write(qPrintable("system_reset\n"));
#endif
    this->state = Machine::Stopped;

    emit(machineStateChangedSignal(Machine::Stopped));
}

/**
 * @brief Reset the machine
 *
 * Reset the machine
 */
void Machine::resetMachine()
{
#ifdef Q_OS_WIN
    if (this->m_machineTcpSocket->state() != QAbstractSocket::ConnectedState) {
        this->failConnectMachine();
    } else {
        this->m_machineTcpSocket->write(qPrintable("system_reset\n"));
    }
#else
    this->m_machineProcess->write(qPrintable("system_reset\n"));
#endif
}

/**
 * @brief Pause the machine
 *
 * If the machine is started, paused it
 * If the machine if paused, started it
 */
void Machine::pauseMachine()
{
    if (state == Machine::Started) {

#ifdef Q_OS_WIN
        if (this->m_machineTcpSocket->state() != QAbstractSocket::ConnectedState) {
            this->failConnectMachine();
        } else {
            this->m_machineTcpSocket->write(qPrintable("stop\n"));
        }
#else
        this->m_machineProcess->write(qPrintable("stop\n"));
#endif
        this->state = Machine::Paused;

        emit(machineStateChangedSignal(Machine::Paused));
    } else if (state == Machine::Paused) {

#ifdef Q_OS_WIN
        if (this->m_machineTcpSocket->state() != QAbstractSocket::ConnectedState) {
            this->failConnectMachine();
        } else {
            this->m_machineTcpSocket->write(qPrintable("cont\n"));
        }
#else
        this->m_machineProcess->write(qPrintable("cont\n"));
#endif
        this->state = Machine::Started;

        emit(machineStateChangedSignal(Machine::Started));
    }
}

/**
 * @brief Read standard output
 *
 * Read the machine standard output
 */
void Machine::readMachineStandardOut()
{
    QByteArray rawStandardOut;
#ifdef Q_OS_WIN
    rawStandardOut = this->m_machineTcpSocket->readAll();
#else
    rawStandardOut = this->m_machineProcess->readAllStandardOutput();
#endif
    QString standardOut = rawStandardOut;
    QStringList splitStandardOut = standardOut.split("[K");
    QString cleanStandardOut = splitStandardOut.last().remove(QRegExp("\\[[KD]."));
    // Remove space characters, included \r \t \n
    cleanStandardOut = cleanStandardOut.simplified();

    if (cleanStandardOut.isEmpty() ||
        cleanStandardOut.contains("(qemu)") ||
        cleanStandardOut.contains("monitor")) {
        return;
    }

    m_machineStandardOutMessageBox = new QMessageBox();
    m_machineStandardOutMessageBox->setWindowTitle(tr("QEMU - Standard Out"));
    m_machineStandardOutMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    m_machineStandardOutMessageBox->setIcon(QMessageBox::Information);
    m_machineStandardOutMessageBox->setText(cleanStandardOut);
    m_machineStandardOutMessageBox->show();
    m_machineStandardOutMessageBox->raise();
    m_machineStandardOutMessageBox->activateWindow();
}

/**
 * @brief Read error output
 *
 * Read the machine error output
 */
void Machine::readMachineErrorOut()
{
    QByteArray rawErrorOutput;

#ifdef Q_OS_WIN
    rawErrorOutput = this->m_machineTcpSocket->readAll();
#else
    rawErrorOutput = this->m_machineProcess->readAllStandardError();
#endif
    QString errorOutput = rawErrorOutput;
    if (errorOutput.isEmpty()) {
        return;
    }

    m_machineErrorOutMessageBox = new QMessageBox();
    m_machineErrorOutMessageBox->setWindowTitle(tr("QEMU - Error Out"));
    m_machineErrorOutMessageBox->setIcon(QMessageBox::Critical);
    m_machineErrorOutMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    m_machineErrorOutMessageBox->setText(errorOutput);
    m_machineErrorOutMessageBox->show();
    m_machineErrorOutMessageBox->raise();
    m_machineErrorOutMessageBox->activateWindow();
}

/**
 * @brief Machine started
 *
 * Emit a signal when the machine is started
 */
void Machine::machineStarted()
{
    this->state = Machine::Started;
    emit(machineStateChangedSignal(Machine::Started));
}

/**
 * @brief Machine finished
 *
 * Emit a signal when the machine is finished
 */
void Machine::machineFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "Exit code: " << exitCode << " exit status: " << exitStatus;
    this->state = Machine::Stopped;
    emit(machineStateChangedSignal(Machine::Stopped));
}

/**
 * @brief Generate the machine command
 * @return List with all the commands
 *
 * Generate the machine command to be executed in QEMU
 */
QStringList Machine::generateMachineCommand()
{
    QStringList qemuCommand;

    #ifdef Q_OS_WIN
    QSettings settings;
    settings.beginGroup("Configuration");
    qemuCommand << "-monitor" << QString("tcp:%1:%2,server,nowait")
                                        .arg(settings.value("qemuMonitorHost", "localhost").toString())
                                        .arg(settings.value("qemuMonitorPort", 6000).toInt());
    settings.endGroup();
    #else
    qemuCommand << "-monitor" << "stdio";
    #endif

    qemuCommand << "-name";
    qemuCommand << this->name;

    QString uuid(this->uuid);
    qemuCommand << "-uuid";
    qemuCommand << uuid.remove("{").remove("}");

    QString accelerators;
    bool firstAccel = true;
    QStringListIterator accelIterator(this->accelerator);
    while (accelIterator.hasNext()) {
        if(firstAccel) {
            firstAccel = false;
        } else {
            accelerators.append(", ");
        }
        accelerators.append(accelIterator.next());
    }

    qemuCommand << "-accel";
    qemuCommand << accelerators;

    QString audioCards;
    bool firstAudio = true;
    QStringListIterator audioIterator(this->audio);
    while (audioIterator.hasNext()) {
        if(firstAudio) {
            firstAudio = false;
        } else {
            audioCards.append(", ");
        }
        audioCards.append(audioIterator.next());
    }

    qemuCommand << "-soundhw";
    qemuCommand << audioCards;

    QString bootOrder;
    QStringListIterator bootIterator(this->m_boot.bootOrder());
    while (bootIterator.hasNext()) {
        bootOrder.append(bootIterator.next());
    }

    QString bootMenu = this->m_boot.bootMenu() ? "on" : "off";

    qemuCommand << "-boot";
    qemuCommand << "order=" + bootOrder + ",menu=" + bootMenu;

    if (this->m_boot.kernelBootEnabled()) {
        if (!this->m_boot.kernelPath().isEmpty()) {
            qemuCommand << "-kernel";
            qemuCommand << this->m_boot.kernelPath();
        }

        if (!this->m_boot.initrdPath().isEmpty()) {
            qemuCommand << "-initrd";
            qemuCommand << this->m_boot.initrdPath();
        }

        if (!this->m_boot.kernelArgs().isEmpty()) {
            qemuCommand << "-append";
            qemuCommand << this->m_boot.kernelArgs();
        }
    }

    qemuCommand << "-m";
    qemuCommand << QString::number(this->RAM);

    qemuCommand << "-k";
    qemuCommand << this->keyboard;

    qemuCommand << "-vga";
    qemuCommand << this->GPUType;

    qemuCommand << "-cpu";
    qemuCommand << this->CPUType;

    QString cpuArgs(QString::number(this->CPUCount));

    cpuArgs.append(",cores=");
    cpuArgs.append(QString::number(this->coresSocket));

    cpuArgs.append(",threads=");
    cpuArgs.append(QString::number(this->threadsCore));


    cpuArgs.append(",sockets=");
    cpuArgs.append(QString::number(this->socketCount));

    cpuArgs.append(",maxcpus=");
    cpuArgs.append(QString::number(this->maxHotCPU));

    qemuCommand << "-smp";
    qemuCommand << cpuArgs;

    QString pipe = this->path;
    pipe.append(QDir::toNativeSeparators("/")).append(this->name).append(".pid");
    qemuCommand << "-pidfile";
    qemuCommand << pipe;

    // Network
    if (this->useNetwork) {
        qemuCommand << "-net";
        qemuCommand << "nic";

        qemuCommand << "-net";
        qemuCommand << "user";
    } else {
        qemuCommand << "-net";
        qemuCommand << "none";
    }

    for (int i = 0; i < media.size(); ++i) {
        QString driveInterface("-");
        driveInterface.append(media.at(i).driveInterface());

        qemuCommand << driveInterface;
        qemuCommand << media.at(i).path();
    }

    qDebug() << "Command " << qemuCommand;

    return qemuCommand;
}

/**
 * @brief Show a message when cannot connect to the machine
 *
 * Show a message when cannot connect to the machine
 */
void Machine::failConnectMachine()
{
    m_failConnectErrorMessageBox = new QMessageBox();
    m_failConnectErrorMessageBox->setWindowTitle(tr("QEMU - Connection"));
    m_failConnectErrorMessageBox->setIcon(QMessageBox::Critical);
    m_failConnectErrorMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
    m_failConnectErrorMessageBox->setText(tr("Fail to send command to the QEMU machine"));
    m_failConnectErrorMessageBox->show();
    m_failConnectErrorMessageBox->raise();
    m_failConnectErrorMessageBox->activateWindow();
}

/**
 * @brief Save the machine
 *
 * Save the machine data in the file of the machine
 */
void Machine::saveMachine()
{
    QFile machineFile(this->configPath);
    if (!machineFile.open(QFile::WriteOnly)) {
        m_saveMachineMessageBox = new QMessageBox();
        m_saveMachineMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_saveMachineMessageBox->setIcon(QMessageBox::Critical);
        m_saveMachineMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_saveMachineMessageBox->setText(tr("<p>Cannot save the machine</p>"
                                            "<p>The file with the machine configuration are not writable</p>"));
        m_saveMachineMessageBox->show();
        m_saveMachineMessageBox->raise();
        m_saveMachineMessageBox->activateWindow();
        return;
    }

    QJsonObject machineJSONObject;
    machineJSONObject["name"]        = this->name;
    machineJSONObject["OSType"]      = this->OSType;
    machineJSONObject["OSVersion"]   = this->OSVersion;
    machineJSONObject["description"] = this->description;
    machineJSONObject["RAM"]         = this->RAM;
    machineJSONObject["network"]     = this->useNetwork;
    machineJSONObject["path"]        = this->path;
    machineJSONObject["uuid"]        = this->uuid;
    machineJSONObject["hostsoundsystem"] = this->hostSoundSystem;
    machineJSONObject["binary"] = "qemu-system-x86_64";

    QJsonObject cpu;
    cpu["CPUType"]     = this->CPUType;
    cpu["CPUCount"]    = this->CPUCount;
    cpu["socketCount"] = this->socketCount;
    cpu["coresSocket"] = this->coresSocket;
    cpu["threadsCore"] = this->threadsCore;
    cpu["maxHotCPU"]   = this->maxHotCPU;
    machineJSONObject["cpu"] = cpu;

    QJsonObject gpu;
    gpu["GPUType"]  = this->GPUType;
    gpu["keyboard"] = this->keyboard;
    machineJSONObject["gpu"] = gpu;

    QJsonArray media;
    for (int i = 0; i < this->media.size(); ++i) {
        QJsonObject disk;
        disk["name"] = this->media.at(i).name();
        disk["path"] = this->media.at(i).path();
        disk["type"] = this->media.at(i).type();
        disk["interface"] = this->media.at(i).driveInterface();
        disk["uuid"] = QUuid::createUuid().toString();

        media.append(disk);
    }

    machineJSONObject["media"] = media;

    QJsonObject kernelBoot;
    kernelBoot["enabled"] = this->m_boot.kernelBootEnabled();
    kernelBoot["kernelPath"] = this->m_boot.kernelPath();
    kernelBoot["initrdPath"] = this->m_boot.initrdPath();
    kernelBoot["kernelArgs"] = this->m_boot.kernelArgs();

    QJsonObject boot;
    boot["bootMenu"] = this->m_boot.bootMenu();
    boot["kernelBoot"] = kernelBoot;
    boot["bootOrder"] = QJsonArray::fromStringList(this->m_boot.bootOrder());

    machineJSONObject["boot"] = boot;

    machineJSONObject["accelerator"] = QJsonArray::fromStringList(this->accelerator);
    machineJSONObject["audio"] = QJsonArray::fromStringList(this->audio);

    QJsonDocument machineJSONDocument(machineJSONObject);

    machineFile.write(machineJSONDocument.toJson());
    machineFile.flush();

    if (machineFile.isOpen()) {
        machineFile.close();
    }

    qDebug() << "Machine saved";
}

/**
 * @brief Insert the new machine in the machines file
 *
 * Insert the new machine in the machines file.
 * At the bottom of the file.
 */
void Machine::insertMachineConfigFile()
{
    QSettings settings;
    settings.beginGroup("DataFolder");
    QString dataDirectoryPath = settings.value("QtEmuData",
                                               QDir::toNativeSeparators(QDir::homePath() + "/.qtemu/")).toString();
    settings.endGroup();
    QString qtemuConfig = dataDirectoryPath.append("qtemu.json");

    QFile machinesFile(qtemuConfig);
    if (!machinesFile.open(QFile::ReadWrite)) {
        m_machineConfigMessageBox = new QMessageBox();
        m_machineConfigMessageBox->setWindowTitle(tr("Qtemu - Critical error"));
        m_machineConfigMessageBox->setIcon(QMessageBox::Critical);
        m_machineConfigMessageBox->setWindowIcon(QIcon::fromTheme("qtemu", QIcon(":/images/qtemu.png")));
        m_machineConfigMessageBox->setText(tr("<p>Cannot save the machine</p>"
                                              "<p>The file with all the machines configuration are not writable</p>"));
        m_machineConfigMessageBox->show();
        m_machineConfigMessageBox->raise();
        m_machineConfigMessageBox->activateWindow();
        return;
    }

    // Read all data included in the file
    QByteArray machinesData = machinesFile.readAll();
    QJsonDocument machinesDocument(QJsonDocument::fromJson(machinesData));
    QJsonObject machinesObject;

    // Read other machines
    QJsonArray machines = machinesDocument["machines"].toArray();

    // Create the new machine
    QJsonObject machine;
    machine["uuid"]       = this->uuid;
    machine["path"]       = this->path;
    machine["configpath"] = this->configPath;
    machine["icon"]       = this->OSVersion.toLower().replace(" ", "_");

    machines.append(machine);
    machinesObject["machines"] = machines;

    QJsonDocument machinesJSONDocument(machinesObject);

    machinesFile.seek(0);
    machinesFile.write(machinesJSONDocument.toJson());
    if (machinesFile.isOpen()) {
        machinesFile.close();
    }
}
