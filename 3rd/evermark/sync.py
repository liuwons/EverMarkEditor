#!/usr/bin/env python
# coding: utf-8

import emark

em = None


def init(account_type, auth_token, style, root_path):
    global em
    em = emark.EverMark(account_type, auth_token, style, root_path)
    return 0


def login():
    print '[DEBUG] sync login'
    global em
    return em.login()


def get_notebook_status():
    print '[DEBUG] sync get_notebook_status'
    global em
    return em.get_notebook_status()


def get_note_status():
    print '[DEBUG] sync get_note_status'
    global em
    return em.get_note_status()


def create_notebook(notebook_name):
    global em
    return em.create_notebook(notebook_name)


def create_note(notebook_guid, note_title, content, is_markdown):
    global em
    return em.create_note(notebook_guid, note_title, content, is_markdown)


def update_note(notebook_guid, guid, note_title, content, is_markdown):
    global em
    return em.update_note(notebook_guid, note_title, content, guid)


def get_note(note_guid):
    print '[DEBUG] sync get_note(%s)' % note_guid
    global em
    return em.get_note(note_guid)

