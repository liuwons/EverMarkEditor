#!/usr/bin/env python
# coding: utf-8

import os
import cgi
import time
import datetime
import json
import traceback

import chardet
import markdown2
import premailer

import evernote.edam.type.ttypes as Types
import evernote.edam.notestore.NoteStore as NoteStore
import evernote.edam.userstore.constants as UserStoreConstants
from evernote.api.client import EvernoteClient


def current_time():
    return datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')


def modify_time(fpath):
    return time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(os.path.getmtime(fpath)))


def compare(str_time1, str_time2):
    time1 = time.strptime(str_time1, '%Y-%m-%d %H:%M:%S')
    time2 = time.strptime(str_time2, '%Y-%m-%d %H:%M:%S')
    timestamp1 = time.mktime(time1)
    timestamp2 = time.mktime(time2)
    return timestamp1 - timestamp2


class EverMark(object):
    def __init__(self, account_type, auth_token, style, root_path):
        self.account_type = account_type
        self.auth_token = auth_token
        self.style = style
        self.root_path = root_path
        self.css_str = ''
        css_file_path = os.path.join(self.root_path, 'css/' + self.style + '.css')
        with open(css_file_path) as f:
            self.css_str = f.read().decode('utf-8')
        self.client = None
        self.user_store = None
        self.note_store = None
        self.notebooks = None

    def login(self):
        print '[DEBUG] emark login'
        yx = False
        if self.account_type == 'yinxiang':
            yx = True
        try:
            self.client = EvernoteClient(token=self.auth_token, sandbox=False, yinxiang=yx)
            self.user_store = self.client.get_user_store()
            version_ok = self.user_store.checkVersion("Evernote EDAMTest (Python)",
                                                      UserStoreConstants.EDAM_VERSION_MAJOR,
                                                      UserStoreConstants.EDAM_VERSION_MINOR)
            if not version_ok:
                return 1
            self.note_store = self.client.get_note_store()
            return 0
        except:
            print traceback.format_exc()
            return 1

    def get_notebook_status(self):
        print '[DEBUG] emark get_notebook_status'
        self.notebooks = self.note_store.listNotebooks()
        notebooks = {}
        for notebook in self.notebooks:
            notebook_stack = ''
            if notebook.stack is not None:
                notebook_stack = notebook.stack.decode('utf-8')
            notebook_name = notebook.name.decode('utf-8')
            notebooks[notebook.guid] = {'name': notebook_name, 'stack': notebook_stack}
        return json.dumps(notebooks).encode('utf-8')

    def get_note_status(self):
        print '[DEBUG] emark get_note_status'
        find_filter = NoteStore.NoteFilter()
        spec = NoteStore.NotesMetadataResultSpec()
        spec.includeTitle = True
        spec.includeNotebookGuid = True
        spec.includeUpdated = True
        spec.includeContentLength = True
        result = {}
        got_count = 0
        while True:
            notes = self.note_store.findNotesMetadata(find_filter, got_count, 10000, spec)
            all_count = notes.totalNotes
            got_count += len(notes.notes)
            for note in notes.notes:
                notebook_guid = note.notebookGuid
                if not notebook_guid:
                    continue
                note_title = note.title.decode('utf-8')
                note_modify_time = time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(int(note.updated/1000)))
                if notebook_guid not in result:
                    result[notebook_guid] = []
                result[notebook_guid].append({'guid': note.guid, 'title': note_title, 'modify_time': note_modify_time})
            if got_count >= all_count:
                break
        return json.dumps(result).encode('utf-8')

    def markdown2html(self, markdown_str):
        markdown_str = markdown_str.decode('utf-8')
        html = u'<style>' + self.css_str
        html += '.markdown-body {box-sizing: border-box;min-width: ' \
                '200px;max-width: 980px;margin: 0 auto;padding: 45px;}'
        html += '</style>'
        html += '<article class="markdown-body">'
        md_html = markdown2.markdown(markdown_str, extras=["tables", "fenced-code-blocks", "cuddled-lists"])
        html += md_html
        html += '</article>'
        prem = premailer.Premailer(html, preserve_inline_attachments=False, base_path='article')
        html = prem.transform(pretty_print=True)
        html = html[html.find('<article'):]
        html = html[html.find('>')+1:]
        html = html[:html.find('</article>')]
        return html.encode('utf-8')

    @staticmethod
    def text2html(text_str):
        text_str = text_str.decode('utf-8')
        text_str.replace('\r', '')
        html = ''
        lines = text_str.split('\n')
        for line in lines:
            lstr = '<div>'
            if not line:
                lstr += '<br />'
            else:
                lstr += cgi.escape(line)
            lstr += '</div>'
            html += lstr
        return html

    def create_notebook(self, notebook_name):
        notebook = Types.Notebook()
        notebook.name = notebook_name.encode('utf-8')
        notebook.defaultNotebook = False
        created_notebook = self.note_store.createNotebook(notebook)
        return 0

    def inner_create_note(self, notebook_guid, note_title, html, guid=None):
        note = Types.Note()
        note.title = note_title
        if guid:
            note.guid = guid
        note.content = '<?xml version="1.0" encoding="UTF-8"?>'
        note.content += '<!DOCTYPE en-note SYSTEM ' \
            '"http://xml.evernote.com/pub/enml2.dtd">'
        note.content += '<en-note>'
        note.content += html
        note.content += '</en-note>'
        note.notebookGuid = notebook_guid
        return note

    def create_note(self, notebook_guid, note_title, content, is_markdown):
        if is_markdown:
            html = self.markdown2html(content)
        else:
            html = self.text2html(content)
        note = self.inner_create_note(notebook_guid, note_title, html)
        created_note = self.note_store.createNote(note)
        return 0

    def update_note(self, notebook_guid, guid, note_title, content, is_markdown):
        if is_markdown:
            html = self.markdown2html(content)
        else:
            html = self.text2html(content)
        note = self.inner_create_note(notebook_guid, note_title, html, guid)
        self.note_store.updateNote(note)
        return 0

    def get_note(self, note_guid):
        print '[DEBUG] emark get_note(%s)' % note_guid
        note = self.note_store.getNote(note_guid, True, True, True, True)
        if note.content is not None:
            return note.content
        else:
            return ''


