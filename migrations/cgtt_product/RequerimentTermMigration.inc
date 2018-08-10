<?php
/**
 * Created by PhpStorm.
 * User: root
 * Date: 1/12/17
 * Time: 8:52
 */

class RequerimentTermMigration extends TaxonomyMigration {
	public function  __construct($arguments) {
		parent::__construct($arguments, 309, 'requerimientos');
	}
}