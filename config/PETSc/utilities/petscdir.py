import config.base
import os
import re

class Configure(config.base.Configure):
  def __init__(self, framework):
    config.base.Configure.__init__(self, framework)
    self.headerPrefix = 'PETSC'
    self.substPrefix  = 'PETSC'
    self.isPetsc      = 1
    return

  def __str1__(self):
    if hasattr(self, 'dir'):
      return '  PETSC_DIR: '+str(self.dir)+'\n'
    return ''

  def setupHelp(self, help):
    import nargs
    help.addArgument('PETSc', '-PETSC_DIR=<root-dir>',                        nargs.Arg(None, None, 'The root directory of the PETSc installation'))
    return

  def setupDependencies(self, framework):
    self.sourceControl = framework.require('config.sourceControl',self)
    self.programs      = framework.require('config.programs',self)
    return

  def configureDirectories(self):
    '''Checks PETSC_DIR and sets if not set'''
    if 'PETSC_DIR' in self.framework.argDB:
      self.dir = self.framework.argDB['PETSC_DIR']
      if self.dir == 'pwd':
        raise RuntimeError('You have set -PETSC_DIR=pwd, you need to use back quotes around the pwd\n  like -PETSC_DIR=`pwd`')
      if not os.path.isdir(self.dir):
        raise RuntimeError('The value you set with -PETSC_DIR='+self.dir+' is not a directory')
    elif 'PETSC_DIR' in os.environ:
      self.dir = os.environ['PETSC_DIR']
      if self.dir == 'pwd':
        raise RuntimeError('''
The environmental variable PETSC_DIR is set incorrectly. Please use the following: [notice backquotes]
  For sh/bash  : PETSC_DIR=`pwd`; export PETSC_DIR
  for csh/tcsh : setenv PETSC_DIR `pwd`''')
      elif not os.path.isdir(self.dir):
        raise RuntimeError('The environmental variable PETSC_DIR '+self.dir+' is not a directory')
    else:
      self.dir = os.getcwd()
    if self.isPetsc and not os.path.realpath(self.dir) == os.path.realpath(os.getcwd()):
      raise RuntimeError('The environmental variable PETSC_DIR '+self.dir+' MUST be the current directory '+os.getcwd())
    if self.dir[1] == ':':
      try:
        dir = self.dir.replace('\\','/')
        (dir, error, status) = self.executeShellCommand('cygpath -au '+dir)
        self.dir = dir.replace('\n','')
      except RuntimeError:
        pass
    self.version  = 'Unknown'
    versionHeader = os.path.join(self.dir, 'include', 'petscversion.h')
    versionInfo = []
    if os.path.exists(versionHeader):
      f = file(versionHeader)
      for line in f:
        if line.find('define PETSC_VERSION') >= 0:
          versionInfo.append(line[:-1])
      f.close()
    else:
      raise RuntimeError('Invalid PETSc directory '+str(self.dir)+'. Could not locate '+versionHeader)
    self.version = '.'.join([line.split(' ')[-1] for line in versionInfo[1:4]])
    self.logPrint('Version Information:')
    for line in versionInfo:
      self.logPrint(line)
    self.addMakeMacro('DIR', self.dir)
    self.addDefine('DIR', '"'+self.dir+'"')
    self.framework.argDB['search-dirs'].append(os.path.join(self.dir, 'bin', 'win32fe'))

    return

  def configureExternalPackagesDir(self):
    if self.framework.externalPackagesDir is None:
      self.externalPackagesDir = os.path.join(self.dir, 'externalpackages')
    else:
      self.externalPackagesDir = self.framework.externalPackagesDir
    return

  def configureInstallationMethod(self):
    if os.path.exists(os.path.join(self.dir,'bin','maint')):
      self.logPrint('bin/maint exists. This appears to be a repository clone')
      self.isClone = 1
      if os.path.exists(os.path.join(self.dir, '.git')):
        if hasattr(self.sourceControl,'git'):
          self.addDefine('VERSION_GIT','"'+os.popen("cd "+self.dir+" && "+self.sourceControl.git+" log -1 --pretty=format:%H").read()+'"')
          self.addDefine('VERSION_DATE_GIT','"'+os.popen("cd "+self.dir+" && "+self.sourceControl.git+" log -1 --pretty=format:%ci").read()+'"')
        else:
          self.logPrintBox('\n*****WARNING: PETSC_DIR appears to be a Git clone - but git is not found in PATH********\n')
      elif os.path.exists(os.path.join(self.dir, '.hg')):
        if hasattr(self.sourceControl,'hg'):
          self.addDefine('VERSION_HG','"'+os.popen(self.sourceControl.hg +" -R"+self.dir+" tip --template '{node}'").read()+'"')
          self.addDefine('VERSION_DATE_HG','"'+os.popen(self.sourceControl.hg +" -R"+self.dir+" tip --template '{date|date}'").read()+'"')
        else:
          self.logPrintBox('\n*****WARNING: PETSC_DIR appears to be a mercurial clone - but hg is not found in PATH********\n')
      else:
        self.logPrint('This repository clone is obtained as a tarball as neither .hg nor .git dirs exist!')
    else:
      if os.path.exists(os.path.join(self.dir, '.git')) or os.path.exists(os.path.join(self.dir, '.hg')):
        raise RuntimeError('Your petsc-dev directory is broken, remove the entire directory and start all over again')
      else:
        self.logPrint('This is a tarball installation')
        self.isClone = 0
    return

  def configure(self):
    self.executeTest(self.configureDirectories)
    self.executeTest(self.configureExternalPackagesDir)
    self.executeTest(self.configureInstallationMethod)
    return
