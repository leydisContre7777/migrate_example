<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 1/12/17
 * Time: 8:52
 */

class SubjectTermMigration extends TaxonomyMigration {
	public function  __construct($arguments) {
		parent::__construct($arguments, 307, 'tem_tica');
	}
}