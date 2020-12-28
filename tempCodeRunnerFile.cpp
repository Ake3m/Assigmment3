    etree.setVar('a', true);
    etree.setVar('b', false);
    etree.setVar('c', false);
    etree.setVar('d', true);
    etree.evaluate(etree.root);
    etree.show();
    cout << "The evaluated result = " << etree.root->result << endl;